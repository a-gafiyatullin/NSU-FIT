(defun len (a) (cond ((not a) 0) ((+ 1 (len (cdr a))))))

(len '())
(len '(1))
(len '(1 2 4 5))
(len '(a))
(len '(a c d))

(defun lastatom (a) (cond ((not (cdr a)) (car a)) ((lastatom (cdr a)))))

(lastatom ())
(lastatom '(1))
(lastatom '(a))
(lastatom '(1 2 3 4))
(lastatom '(a b c d))

(defun findall_impl (l elem &optional (pos 0))
    (
        cond ((not l)
            ()
        )
        ((eq (car l) elem)
            (cons pos (findall_impl (cdr l) elem (+ 1 pos)))
        )
        (
            (findall_impl (cdr l) elem (+ 1 pos))
        )
    )
)

(findall_impl '(1 2 3 1) 1)
(findall_impl '(1 2 10 1 5 10) 0)
(findall_impl '(a b c a d) 'a)

(defun delelem (l elem)
    (
        cond ((not l)
            ()
        )
        ((eq (car l) elem)
            (delelem (cdr l) elem)
        )
        (
            (cons (car l) (delelem (cdr l) elem))
        )
    )
)

(defun delelems (l elems)
    (
        cond ((not elems)
            l
        )
        (
            (delelems (delelem l (car elems)) (cdr elems))
        )
    )
)

(delelems '(1 2 3 10 4 6 10 1 ) '(1 10))
(delelems '(a b c a b e l d ) '(a e))