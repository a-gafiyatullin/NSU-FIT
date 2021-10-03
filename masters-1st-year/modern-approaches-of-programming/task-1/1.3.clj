(defn my-map [f coll]
    (reduce (fn [x y] (concat x (list (f y)))) () coll)
)

(println (my-map dec (list 1 2 3 4 5)))

(defn my-filter [pred coll]
    (reduce (fn [x y]
                (if (pred y)
                    (concat x (list y))
                    x
                )
            ) () coll
    )
)

(print (my-filter (fn [x] (= 0 (mod x 3))) (list 1 2 3 4 5)))