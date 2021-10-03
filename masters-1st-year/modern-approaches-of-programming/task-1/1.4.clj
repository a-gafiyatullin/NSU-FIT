(defn l-to-l-of-l [l]
    (reduce (fn [x y] (cons (list y) x)) () l)
)

(defn append [c ls]
    (map (fn [x] (concat x (list c))) (filter (fn [x] (not= c (last x))) ls))
)

(defn append-all [lc ls]
    (reduce (fn [x y] (concat x (append y ls))) () lc)
)

(defn cons-perms [n l]
    (reduce (fn [x y] (append-all l x)) (l-to-l-of-l l) (range 1 n))
)

(print (cons-perms 2 '(a b c)))