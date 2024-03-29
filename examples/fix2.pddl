(define (problem fix2)
  (:domain init-flat-tire)
  (:objects wheel1 wheel2 hub nuts boot)
  (:init (wheel wheel1)
	 (wheel wheel2) (hub hub) (nut nuts)
	 (container boot) (intact wheel2)
	 (open boot)
	 (have jack) (have pump) (have wheel2) (have wrench)
	 (on wheel1 hub) (on-ground hub) (tight nuts hub))
  (:goal (and (inflated wheel2) (not (on-ground hub))
	      (loose nuts hub))))
