(ns task-4.core
  (:gen-class))

; constant
(defn const [value]
  {:pre [(boolean? value)]}
  (list ::const value))

(defn const? [expr]
  (= (first expr) ::const))

(defn const-value [c]
  {:pre [(const? c)]}
  (second c))

; variable
(defn variable [name]
  {:pre [(not (keyword? name))]}
  (list ::var name))

(defn variable? [expr]
  (= (first expr) ::var))

(defn variable-name [v]
  {:pre [(not (var? v))]}
  (second v))

(defn same-variable [v1 v2]
  {:pre [(and (variable? v1) (variable? v2))]}
  (=
   (variable-name v1)
   (variable-name v2)))

; conjunction
(defn conjunction [expr_list]
  (cons ::conj expr_list))

(defn conjunction? [expr]
  (= (first expr) ::conj))

; disjunction
(defn disjunction [expr_list]
  (cons ::disj expr_list))

(defn disjunction? [expr]
  (= (first expr) ::disj))

; inversion
(defn inversion [expr]
  (list ::inv expr))

(defn inversion? [expr]
  (= (first expr) ::inv))

(defn inversion-expr [expr]
  {:pre [inversion expr]}
  (second expr))

; implication
(defn implication [expr_list]
  (cons ::impl expr_list))

(defn implication? [expr]
  (= (first expr) ::impl))

; get op args
(defn args [expr]
  {:pre [(or
          (conjunction? expr)
          (disjunction? expr)
          (implication? expr))]}
  (rest expr))

(defn expr-type [expr] (first expr))

(defn equals [expr1 expr2]
  (if (=
       (expr-type expr1)
       (expr-type expr2))
    (cond
      (const? expr1) (=
                      (const-value expr1)
                      (const-value expr2))
      (variable? expr1) (=
                         (variable-name expr1)
                         (variable-name expr2))
      (inversion? expr1) (equals
                          (inversion-expr expr1)
                          (inversion-expr expr2))
      (or
       (disjunction? expr1)
       (conjunction? expr1)
       (implication? expr1)) (every?
                              #(equals (first %) (second %))
                              (map vector
                                   (args expr1)
                                   (args expr2))))
    false))

; rules
(declare dnf)

(def rules
  (list
   [const? identity]
   [variable? identity]
   ; A | (B | C), (A | B) | C -> A | B | C
   [disjunction?
    (fn [expr]
      (disjunction (mapcat #(if (disjunction? %)
                              (map dnf (args %))
                              (list (dnf %)))
                           (args expr))))]
   [conjunction?
    (fn [expr]
      ; A & (B & C), (B & C) is not in dnf -> A & (B' | C')
      (let [args-to-dnf (mapcat #(if (conjunction? %)
                                   (map dnf (args %))
                                   (list (dnf %)))
                                (args expr))
      ; A & (B' | C') -> A & B' | A & C'
            some-disjunct (first (filter disjunction? args-to-dnf))]
        (if (nil? some-disjunct)
          (conjunction args-to-dnf)
          (let [other-args (first (filter #(not (identical? some-disjunct %)) args-to-dnf))]
            (dnf (disjunction (map #(dnf (conjunction (list % other-args))) (args some-disjunct))))))))]
   [inversion?
    (fn [arg]
      (let [expr (inversion-expr arg)]
        (cond
          (const? expr) arg
          (variable? expr) arg
          ; - - A = A
          (inversion? expr) (dnf (inversion-expr expr))
          ; - (A | B) = -A & -B
          (disjunction? expr) (dnf (conjunction (map
                                                 #(dnf (inversion %))
                                                 (args expr))))
          ; - (A & B) = - A | - B
          (conjunction? expr) (dnf (disjunction (map
                                                 #(dnf (inversion %))
                                                 (args expr))))
           ; ¬ (A -> B)  = A & ¬B
          (implication? expr) (dnf (conjunction
                                    (list (first (args expr))
                                          (inversion (second (args expr)))))))))]
   ; A -> B = - A | B
   [implication? (fn [expr] (dnf (disjunction
                                  (list (inversion (first (args expr)))
                                        (second (args expr))))))]))

(declare simplify)

; rules for dnf simplifying
(def simplify-rules
  (list
   [const? identity]
   [variable? identity]
   [disjunction?
    (fn [expr]
      (let [args (filter #(not (nil? %)) (map simplify (args expr)))
            ; at least one true -> disjunction is true
            one-true (filter #(and (const? %) (equals % (const true))) args)]
        (if (nil? (first one-true))
          (if (= (count args) 1) (first args) (disjunction args))
          (const true))))]
   [conjunction?
    (fn [expr]
      (let [inversed (map simplify (args expr))
            ; at least one false -> conjunction is false
            one-false (filter #(and (const? %) (equals % (const false))) inversed)
            any-vars (filter #(or (variable? %) (inversion? %)) inversed)]
        (if (not (nil? (first one-false)))
          nil
          (if (nil? (first any-vars))
            (const true)
            (if (= (count any-vars) 1) (first any-vars) (conjunction any-vars))))))]
   [inversion?
    (fn [arg]
      (let [expr (inversion-expr arg)]
        (cond
          ; -1 -> 0, -0 -> 1
          (const? expr) (if (equals expr (const true)) (const false) (const true))
          (variable? expr) arg)))]))

; simplify dnf
(defn simplify [dnf-expr]
  (if (nil? dnf-expr)
    dnf-expr
    ((some (fn [[check, action]]
             (if (check dnf-expr)
               action
               false))
           simplify-rules)
     dnf-expr)))

; compute dnf
(defn dnf [expr]
  (simplify ((some (fn [[check, action]]
                     (if (check expr)
                       action
                       false))
                   rules)
             expr)))

(declare apply-variable-value)

; rules for variable applying
(def apply-var-rules
  (list
   [const? identity]
   [variable?
    (fn [expr vari value]
      (if (same-variable expr vari) value expr))]
   [disjunction?
    (fn [expr var value]
      (disjunction (map #(apply-variable-value % var value) (args expr))))]
   [conjunction?
    (fn [expr var value]
      (conjunction (map #(apply-variable-value % var value) (args expr))))]
   [inversion?
    (fn [expr var value]
      (inversion (apply-variable-value (inversion-expr expr) var value)))]))

; set variable "var" to value "value"
(defn apply-variable-value [expr var value]
  {:pre [(and
          (variable? var)
          (const? value))]}
  (simplify ((some (fn [[check, action]]
                     (if (check expr)
                       action
                       false))
                   apply-var-rules)
             expr var value)))
