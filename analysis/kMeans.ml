open Batteries

module type CLUSTER = sig
  type centroid
  type at

  val distance : at -> at -> float
  val at : centroid -> at
  val average : at array -> at option
  val move : centroid -> at -> centroid
  val generate : int -> centroid

  val add : at -> at -> at
  val origo : at
end

module Cluster1D =
struct
  type centroid = float
  type at = float

  let distance a b = (a -. b ) ** 2.0
  let at x = x
  let average xs = 
    if Array.length xs = 0
    then None 
    else Some (St.mean xs)
  let move x towards = towards
  let generate i = Random.float 0.5 +. float i
  let add x y = x +. y
  let origo = 0.0
end 

module Cluster1DTest = (Cluster1D: CLUSTER)

let array_extreme_at compare array =
  let (min_i, _) =
    Array.fold_lefti 
      (fun (min_i, min_value) index value -> 
	if compare value min_value 
	then (index, value)
	else (min_i, min_value)) 
      (0, array.(0))
      array
  in
  min_i

let array_extreme_at2 (mk_comparable : 'a -> 'b) (compare : 'b -> 'b -> bool) (array : 'a array) : int =
  let a = Array.map (fun v -> (mk_comparable v, v)) array in
  array_extreme_at (fun (a, _) (b, _) -> compare a b) a

let array_minimum_at = array_extreme_at (<)

let some x = Some x

module Make = functor (C : CLUSTER) -> struct
  let random_centroids num_clusters = Array.init num_clusters C.generate

  let fixed_centroids num_clusters data =
    Array.filter_map identity (
      Array.init num_clusters 
	(fun i -> 
	  if i >= Array.length data
	  then None
	  else 
	    let centroid = C.generate i in
	    let centroid = C.move centroid data.(i) in
	    Some centroid
	)    
    )

  let cost centroids values =
    let sum =
      Array.fold_left
	(fun sum (value, cluster_idx) -> sum +. C.distance value (C.at centroids.(cluster_idx)))
	0.0
	values
    in
    sum

  let cluster_of centroids value =
    let distances =
      Array.map 
	(fun cluster ->
	  match cluster with
	    | None -> infinity
	    | Some cluster -> C.distance (C.at cluster) value)
	centroids in
    array_minimum_at distances

  (* refactor with cluster_of.. *)
  let cluster_of' centroids value =
    let distances =
      Array.map 
	(fun cluster -> C.distance (C.at cluster) value)
	centroids in
    array_minimum_at distances

  let kmeans_centroids ?(max_iterations=100) centroids (data : C.at array) =
    (* values associated to their cluster *)
    let centroids = Array.map (fun v -> Some v) centroids in
    let values = Array.map (fun value -> (value, cluster_of centroids value)) data in
    let cluster_center (cluster_idx : int) =
      let cluster_members =
	Array.filter_map 
	  (fun (value, cluster_idx') -> 
	    if cluster_idx = cluster_idx'
	    then Some value
	    else None)
	  values in
      C.average cluster_members
    in
    let rec iterate num_iteration =
      if num_iteration >= max_iterations then
	Array.filter_map identity centroids
      else begin
	(* move cluster points towards cluster centers *)
	for i = 0 to Array.length centroids - 1 do
	  match centroids.(i) with
	    | None -> ()
	    | Some centroid ->
	      match cluster_center i with
		| None -> centroids.(i) <- None
		| Some center -> centroids.(i) <- Some (C.move centroid center);
	done;

	(* re-assign value centroids *)
	for i = 0 to Array.length data - 1 do
	  let (value, _) = values.(i) in
	  let cluster_idx = cluster_of centroids value in
	  values.(i) <- (value, cluster_idx)
	done;
	iterate (num_iteration + 1)
      end
    in 
    iterate 1

  let cluster_cost centroids data =
    cost centroids (Array.map (fun v -> (v, cluster_of (Array.map some centroids) v)) data)

  let kmeans ?(num_tries=10) ?max_iterations num_clusters (data : C.at array) =
    let tries = [kmeans_centroids ?max_iterations (fixed_centroids num_clusters data) data] in
    let tries = 
      tries @ 
	(Enum.range ~until:num_tries 1 
	    |> Enum.map (fun _ -> kmeans_centroids ?max_iterations (random_centroids num_clusters) data) 
	    |> List.of_enum) in
    let tries = Array.of_list tries in
    let mapping centroids = cluster_cost centroids data in
    tries.(
      array_extreme_at2 
	mapping
	(<)
	tries
    )
      
  (* Uses the elbow method *)
  let nmeans ?num_tries ?max_iterations (data : C.at array) =
    let rec iterate num_clusters prev =
      let centroids = kmeans ?num_tries ?max_iterations num_clusters data in
      let cost = cluster_cost centroids data in
      match prev with
	| (cost', _)::(cost'', _)::_ when ((log (cost /. cost'))/.(log (cost' /. cost''))) > 10.0 ->
	  centroids
	| _ when num_clusters < 7 ->
	  iterate (num_clusters + 1) ((cost, centroids)::prev)
	| _ -> centroids
    in
    iterate 1 []

  (* Uses the elbow method *)
  let bestmeans ?num_tries ?max_iterations num_clusters_list (data : C.at array) =
    snd (
      List.hd (
	List.sort compare (
	  List.map 
	    (fun num_clusters ->
	      let centroids = kmeans ?num_tries ?max_iterations num_clusters data in
	      (cluster_cost centroids data, centroids)
	    )
	    num_clusters_list
	)
      )
    )    
end

module KMeansFloat = Make(Cluster1D)
