(defn l-to-l-of-l [l]
    (if (= l ())
        ()
        (cons (list (first l)) (l-to-l-of-l (rest l)))
    )
)

(defn append [c ls]
    (if (= ls ())
        ()
        (if (= c (last (first ls)))
            (append c (rest ls))
            (cons (concat (first ls) (list c)) (append c (rest ls)))
        )
    )
)

(defn append-all [lc ls]
    (if (= lc ())
        ()
        (concat (append (first lc) ls) (append-all (rest lc) ls))
    )
)

(defn cons-perms
    ([n l]
        (if (= n 0)
            ()
            (cons-perms l (dec n) (l-to-l-of-l l))
        )
    )
    ([l n r]
        (if (= n 0)
            r
            (cons-perms l (dec n) (append-all l r))
        )
    )
)

(print (cons-perms 2 '(a b c)))