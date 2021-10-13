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