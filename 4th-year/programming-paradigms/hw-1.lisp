(set 'a 10)
(set 'b 2)

(set 'f_a 10.5)
(set 'f_b 2.2)
(set 'f_c 5.5)
(set 'f_d 3.8)
(set 'f_e 3.8)
(set 'f_f 10.5)

(defun QUOTIENT (a b) (/ a b))
(defun <> (a b) (if (eq a b) () T))

(prin1 '+\ )
(prin1 (+ a b))
(terpri)

(prin1 '-\ )
(prin1 (- a b))
(terpri)

(prin1 '*\ )
(prin1 (* a b))
(terpri)

(prin1 'QUOTIENT\ )
(prin1 (QUOTIENT a b))
(terpri)

(prin1 '%\ )
(defun % (a b) (- a (* b (/ a b))))
(prin1 (% a b))
(terpri)

(prin1 '/\ )
(prin1 (/ f_a f_b f_c f_d))
(terpri)

(prin1 'expt\ )
(prin1 (expt a b))
(terpri)


(prin1 '>\ )
(prin1 (> f_a f_c f_d f_e))
(terpri)
(prin1 '>\ )
(prin1 (> f_a f_c f_d f_b))
(terpri)

(prin1 '>=\ )
(prin1 (>= f_a f_b f_c f_d))
(terpri)
(prin1 '>=\ )
(prin1 (>= f_a f_c f_d f_e))
(terpri)

(prin1 '<\ )
(prin1 (< f_b f_d f_c f_a f_f))
(terpri)
(prin1 '<\ )
(prin1 (< f_b f_d f_c f_a))
(terpri)

(prin1 '<=\ )
(prin1 (<= f_a f_b f_c f_d))
(terpri)
(prin1 '<=\ )
(prin1 (<= f_b f_d f_c f_a f_f))
(terpri)

(prin1 '=\ )
(prin1 (= f_a f_b f_c f_d))
(terpri)
(prin1 '=\ )
(prin1 (= f_b f_b f_b))
(terpri)

(prin1 '<>\ )
(prin1 (<> f_a f_b))
(terpri)
(prin1 '<>\ )
(prin1 (<> f_b f_b))
(terpri)