(defun to_atoms (a)
    (
        cond (
            (not a)
                ()
        )
        (
            (atom (car a))
                (cons (car a) (to_atoms (cdr a)))
        )
        (
            (append (to_atoms (car a)) (to_atoms (cdr a)))
        )
    )
)

(to_atoms ())
(to_atoms '(1 (2)))
(to_atoms '(1 2 3 (4 5 (NIL))))
(to_atoms '(a b c d (e) (f g (h))))

(defun len (a) (cond ((not a) 0) ((+ 1 (len (cdr a))))))

(defun len_nonlinear (a) (len (to_atoms a)))

(len_nonlinear ())
(len_nonlinear '(1 (2)))
(len_nonlinear '(1 2 3 (4 5 (8))))
(len_nonlinear '(a b c d (e) (f g (h))))

(defun eq_lists (a b)
    (
        cond
        (
            (not a)
            (
                cond
                (
                    (not b)
                        T
                )
                (
                    ()
                )
            )
        )
        (
            (not b)
            (
                cond
                (
                    (not a)
                        T
                )
                (
                    ()
                )
            )
        )
        (
            (atom a)
            (
                cond
                (
                    (atom b)
                    (
                        cond
                        (
                            (eq a b)
                            T
                        )
                    )
                )
                (
                    ()
                )
            )
        )
        (
            (atom b)
            (
                cond
                (
                    (atom a)
                    (
                        cond
                        (
                            (eq a b)
                            T
                        )
                    )
                )
                (
                    ()
                )
            )
        )
        (
            (atom (car a))
            (
                cond
                (
                    (atom (car b))
                    (
                        cond
                        (
                            (eq (car a) (car b))
                                (eq_lists (cdr a) (cdr b))
                        )
                        (
                            ()
                        )
                    )
                )
                (
                    ()
                )
            )
        )
        (
            (
                cond
                (
                    (atom (car b))
                        ()
                )
                (
                    (eq_lists (car a) (car b))
                        (eq_lists (cdr a) (cdr b))
                )
            )
        )
    )
)

(defun search_elem (l a &optional (pos 1) (ans ()))
    (
        cond
        (
            (eq_lists l a)
            (list ans)
        )
        (
            (
                cond
                (
                    (atom l)
                    NIL
                )
                (
                    (atom (car l))
                    (
                        cond
                        (
                            (eq (cdr l) 'NIL)
                            (search_elem (car l) a pos (append ans (list pos)))
                        )
                        ((append (search_elem (car l) a pos (append ans (list pos))) (search_elem (cdr l) a (+ 1 pos) ans)))
                    )
                )
                (
                    (
                        cond
                        (
                            (eq (cdr l) 'NIL)
                            (search_elem (car l) a 1 (append ans (list pos)))
                        )
                        ((append (search_elem (car l) a 1 (append ans (list pos))) (search_elem (cdr l) a (+ 1 pos) ans)))
                    )
                )
            )
        )
    )
)

 (search_elem '(a (b a (c d e)) a) 'a)
 (search_elem '(a b c a) 'a)
 (search_elem '(a NIL c a d NIL) 'a)
 (search_elem '(a b c (d e) l (m n (o p (d e)))) 'a)
 (search_elem '(a b c (d e) l (m n (o p (d e)))) 'm)
 (search_elem '(a b c (d e) l (m n (o p (d e))) (o p (d e))) '(d e))
 (search_elem '(a b c (d e) l (m n (o p (d e))) (o p (d e))) '(o p (d e)))

(defun insert_elem (l a e)
    (
        cond
        (
            (atom l)
            l
        )
        (
            (eq_lists (car l) a)
            (append (append (list e) (list a)) (insert_elem (cdr l) a e))
        )
        (
           (append (list (insert_elem (car l) a e)) (insert_elem (cdr l) a e))
        )
    )
)


(insert_elem '(1 2 3 2) '2 '0)
(insert_elem '(1 (2 1) 3 (2 1)) '(2 1) 'NIL)
(insert_elem '(1 NIL 3 NIL) 'NIL '1)
(insert_elem '(1 (2 1) 3 (2 1)) '(2 1) '(0))
(insert_elem '(1 (2 1 (3)) 3 (2 1 (3)) (3)) '(3) '(5 2 8))
(insert_elem '(1 (2 1 (3)) 3 (2 1 (3 0 (1))) (3)) '(3 0 (1)) '(5 2 8 (6)))
(insert_elem '(1 (2 1 (3)) NIL (2 1 (3 0 (1))) (NIL)) '(3 0 (1)) '(5 2 8 (6)))