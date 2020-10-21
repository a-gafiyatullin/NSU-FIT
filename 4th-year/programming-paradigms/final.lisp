(defun create_path (l &optional (a 0))
    (
        cond
        (
            (atom (car l))
            (
                cond
                (
                    (eq l NIL)
                    'A
                )
                (
                    (eq (- (car l) a) 1)
                    (list (create_path (cdr l)))
                )
                (
                    (append (list 'x) (create_path (append (list (- (car l) 1)) (cdr l)) a))
                )
            )
        )
        (
            (eq (cdr l) NIL)
            (create_path (car l) a)
        )
        (
            (append (create_path (car l) a) (create_path (cdr l) (car (car l))) )
        )
    )
)

(create_path '((1) (2 2) (3)))
(create_path '((1) (4)))
(create_path '((4) (6 3 3) (7 3)))
(create_path '((6 3) (7)))

