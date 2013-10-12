open Batteries

module IntSet = Set.Make(Int)
module FloatSet = Set.Make(Float)

let floats_of filename =
  let data = File.lines_of filename in
    Enum.map Float.of_string data

module K = KMeans.KMeansFloat

let seqlen common source =
  let data = Array.of_enum (floats_of source) in
  let data_a = Array.init (Array.length data / 2) (fun i -> data.(i * 2)) in
  let data_b = Array.init (Array.length data / 2) (fun i -> data.(i * 2 + 1)) in
  let find_centroids label data =
    let centroids = K.bestmeans [1; 2; 3; 4; 5; 6] data in
    Array.sort compare centroids;
    Array.iteri
      (fun idx centroid ->
	Printf.printf "%s %f " label centroid;
	let centroid_data = Array.filter (fun value -> K.cluster_of' centroids value = idx) data in
	Printf.printf "n %d mean %f stddev %f min %f max %f\n" 
	  (Array.length centroid_data) (St.mean centroid_data) (St.stddev centroid_data)
	  (Array.fold_left min centroid_data.(0) centroid_data) (Array.fold_left max centroid_data.(0) centroid_data) 
	(* Array.iter  *)
	(*   (fun value -> *)
	(*     Printf.printf " %d" (int_of_float value) *)
	(*   ) *)
	(*   centroid_data; *)
	(* Printf.printf "\n"; *)
      )
      centroids
  in
  find_centroids "1" data_a;
  find_centroids "0" data_b;

      

    
