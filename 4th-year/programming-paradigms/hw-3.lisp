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
(to_atoms '(1 2 3 (4 5 (8))))
(to_atoms '(a b c d (e) (f g (h))))

(defun len (a) (cond ((not a) 0) ((+ 1 (len (cdr a))))))

(defun len_nonlinear (a) (len (to_atoms a)))

(len_nonlinear ())
(len_nonlinear '(1 (2)))
(len_nonlinear '(1 2 3 (4 5 (8))))
(len_nonlinear '(a b c d (e) (f g (h))))