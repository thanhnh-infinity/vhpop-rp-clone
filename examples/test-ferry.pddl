(define (problem test-ferry)
  (:domain ferry-domain)
  (:objects a b c1 c2)
  (:init (place a) (place b) (auto c1) (auto c2)
	 (at c1 a) (at c2 a) (at-ferry a)
	 (empty-ferry))
  (:goal (and (at c1 b) (at c2 b))))
