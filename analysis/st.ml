let count data = float (Array.length data)

let sum data = Array.fold_left (+.) 0.0 data

let mean data = sum data /. count data

let stddev data = 
  let avg = mean data in
  let data2 = Array.map (fun v -> (v -. avg) ** 2.0) data in
  let sum = sum data2 in
  (sum /. (count data -. 1.0)) ** 0.5

let normalize data =
  let avg = mean data in
  let mn = Array.map (fun v -> v -. avg) data in
  let sigma = stddev data in
  let norm = Array.map (fun v -> v /. sigma) mn in
  norm

