(ns task-4.core-test
  (:require [clojure.test :refer :all]
            [task-4.core :refer :all]))

(def true-const (const true))
(def false-const (const false))

(def A (variable 'A))
(def B (variable 'B))
(def C (variable 'C))
(def D (variable 'D))

(deftest equals_const_test
  (is (equals
       (disjunction (list true-const false-const))
       (disjunction (list true-const false-const)))))

(deftest equals_const_false_test
  (is (not (equals
            (disjunction (list true-const false-const))
            (disjunction (list true-const true-const))))))

(deftest equals_var_test
  (is (equals
       (disjunction (list A B))
       (disjunction (list A B)))))

(deftest equals_var_false_test
  (is (not (equals
            (disjunction (list A B))
            (disjunction (list B B))))))

(deftest disjunction-dnf-test
  (is (equals
       (disjunction (list A B))
       (dnf (disjunction (list A B))))))

(deftest conjunction-dnf-test
  (is (equals
       (conjunction (list A B))
       (dnf (conjunction (list A B))))))

(deftest conjunction-dnf-test-complex
  (is (equals
       (disjunction
        (list
         (conjunction (list C A))
         (conjunction (list D A))
         (conjunction (list C B))
         (conjunction (list D B))))
       (dnf (conjunction (list (disjunction (list A B)) (disjunction (list C D))))))))

(deftest double-inversion-test
  (is (equals A (dnf (inversion (inversion A))))))

(deftest disjunction-inversion-test
  (is (equals
       (conjunction (list (inversion A) (inversion B)))
       (dnf (inversion (disjunction (list A B)))))))

(deftest conjunction-inversion-test
  (is (equals
       (disjunction (list (inversion A) (inversion B)))
       (dnf (inversion (conjunction (list A B)))))))

(deftest implication-inversion-test
  (is (equals
       (conjunction (list A (inversion B)))
       (dnf (inversion (implication (list A B)))))))

(deftest implication-test
  (is (equals
       (disjunction (list (inversion A) B))
       (dnf (implication (list A B))))))

(deftest simplify-test
  (is (equals
       (disjunction (list C D))
       (dnf (conjunction
             (list
              (disjunction (list (inversion false-const) (inversion true-const)))
              (disjunction (list C D))))))))

(deftest apply-simple-test
  (is (equals true-const (apply-variable-value A A true-const))))

(deftest apply-complex-test
  (is (equals
       (disjunction (list C D))
       (apply-variable-value (apply-variable-value (dnf (conjunction
                                                         (list
                                                          (disjunction (list A B))
                                                          (disjunction (list C D))))) A true-const) B false-const))))

(deftest apply-complex-with-inversion-test
  (is (equals
       (disjunction (list B (conjunction (list D B))))
       (apply-variable-value (apply-variable-value (dnf (conjunction
                                                         (list
                                                          (disjunction (list (inversion A) B))
                                                          (disjunction (list (inversion C) D))))) A true-const) C false-const))))