(ns task-2.core-test
  (:require [clojure.test :refer :all]
            [task-2.core :refer :all]))

(deftest term-test-const
  (testing "Term test for const func"
    (is (= (term (fn [x] 1) 0 5.0) 5.0))
    )
  )

(deftest term-test-x
  (testing "Term test for x func"
    (is (= (term (fn [x] x) 0.0 5.0) 12.5))
    )
  )

(deftest integral-result-const
  (let [integral (make-memoized-integral-func (fn [x] 1))]
    (is ( = (integral 100.0) 100.0))
    )
  )

(deftest integral-result-x
  (let [integral (make-memoized-integral-func (fn [x] x))]
    (is (= (integral 100.0) 5000.0))
    )
  )

(deftest lazy-integral-result-const
  (let [integral (make-lazy-integral-func (fn [x] 1))]
    (is (= (integral 100.0) 100.0))))

(deftest lazy-integral-result-x
  (let [integral (make-lazy-integral-func (fn [x] x))]
    (is (< (- (integral 100.0) 5000.0) 0.1))))

(deftest non-memoized-integral
  (let [integral (make-usual-integral-func (fn [x] (Math/sin x)))]
    (time (integral 100.0))
    (time (integral 120.0))
    )
  )

(deftest memoized-integral
  (let [integral (make-memoized-integral-func (fn [x] (Math/cos x)))]
    (time (integral 100.0))
    (time (integral 120.0))
    )
  )

(deftest lazy-integral
  (let [integral (make-lazy-integral-func (fn [x] (Math/cos x)))]
    (time (integral 100.0))
    (time (integral 100.0))
    (time (integral 120.0))))
