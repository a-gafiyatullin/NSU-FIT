(ns task-3.core-test
  (:require [clojure.test :refer :all]
            [task-3.core :refer :all]))

(defn my-even [x]
  (Thread/sleep 10)
  (even? x))

(deftest parallel-filter-test
  (is (= (doall (filter even? (range 0 1000)))
         (doall (parallel-filter even? (range 0 1000) 100))))
  (time (doall (filter my-even (range 0 1000))))
  (time (doall (parallel-filter my-even (range 0 1000) 100))))

(deftest parallel-lazy-filter-test
  (is (= (doall (filter even? (range 0 1000)))
         (doall (parallel-lazy-filter even? (range 0 1000) 25 4))))
  (time (doall (filter my-even (range 0 1000))))
  (time (doall (parallel-lazy-filter my-even (range 0 1000) 25 4))))

;(deftest parallel-lazy-filter-test-infinite
;  (print (parallel-lazy-filter even? (range) 25 4)))

;(deftest parallel-filter-test-infinite
;  (print (parallel-filter even? (range) 100)))