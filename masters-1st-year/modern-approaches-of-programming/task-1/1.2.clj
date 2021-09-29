(defn l-to-l-of-l
    ([l] (l-to-l-of-l l ()))
    ([l res]
        (if (= l ())
            res
            (recur (rest l) (conj res (list (first l))))
        )
    )
)

(defn append
    ([c ls] (append c ls ()))
    ([c ls res]
        (if (= ls ())
            res
            (if (= c (last (first ls)))
                (recur c (rest ls) res)
                (recur c (rest ls) (cons (concat (first ls) (list c)) res))
            )
        )
    )
)

(defn append-all
    ([lc ls] (append-all lc ls ()))
    ([lc ls res]
        (if (= lc ())
            res
            (recur (rest lc) ls (concat res (append (first lc) ls)))
        )
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
            (recur l (dec n) (append-all l r))
        )
    )
)

(print (cons-perms 2 '(a b c)))