(define (problem tower-invert4)
  (:domain blocks-world-domain)
  (:objects a b c d)
  (:init (block a) (block b) (block c) (block d) (block table)
	 (on a b) (on b c) (on c d) (on d table)
	 (clear a) (clear table))
  (:goal (and (on b c) (on c d) (on d a))))
