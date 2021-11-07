(ns task-3.core
  (:gen-class))

(defn my-filter [pred coll chunk_size i]
  (->> coll
       (drop (* chunk_size i))
       (take chunk_size)
       (filter pred)
       (doall)))

(defn parallel-filter [pred coll chunk_size]
  (->> (range 0 (/ (count coll) chunk_size))
       (map #(future (my-filter pred coll chunk_size %)))
       (mapcat deref)))

(defn distribute_parallel_chunk [chunk_num parallel_chunk_size chunk_size_per_thread threads-num pred coll]
  (let [seq (take parallel_chunk_size (drop (* chunk_num parallel_chunk_size) coll))]
  (->> (range 0 threads-num)
       (map #(future (my-filter pred seq chunk_size_per_thread %)))
       (doall)
       (mapcat deref))))

(defn parallel-lazy-filter [pred coll chunk_size_per_thread threads-num]
  (let [parallel_chunk_size (* chunk_size_per_thread threads-num)]
    (->> (if (counted? coll) (range 0 (/ (count coll) parallel_chunk_size)) (range))
         (mapcat #(distribute_parallel_chunk % parallel_chunk_size chunk_size_per_thread threads-num pred coll)))))
