(ns task-2.core
  (:gen-class))

(def h 0.5)

(defn term [f a b]
  (* (/ (+ (f a) (f b)) 2) (- b a))
  )

(defn- integrate [integrate-helper f x]
  (let [i (int (/ x h))]
    (+ (term f (* h i) x) (integrate-helper integrate-helper i)
       )
    )
  )

(defn make-memoized-integral-func [f]
  (partial integrate
           (memoize (fn [integrate-helper i]
                      (if (> i 0)
                        (+ (term f (* h (dec i)) (* h i))
                           (integrate-helper integrate-helper (dec i))
                           )
                        0.0
                        )
                      )
                    )
           f)
  )

(defn make-usual-integral-func [f]
  (partial integrate
           (fn [integrate-helper i]
                      (if (> i 0)
                        (+ (term f (* h (dec i)) (* h i))
                           (integrate-helper integrate-helper (dec i))
                           )
                        0.0
                        )
             )
           f)
  )

(def h2 0.001)

(defn part_sum_seq [f]
  (iterate
   (fn [[i, value]] (list
                     (inc i)
                     (+ value (term f (* i h2) (* (inc i) h2)))
                     )
     )
   (list 0 0.0)
   )
  )

(defn make-lazy-integral-func [f]
  (let [part-sums (part_sum_seq f)]
    (fn [x]
      (let [step (int (/ x h2))]
        (+ (nth (nth part-sums step) 1)
           (term f (* step h2) x))))))