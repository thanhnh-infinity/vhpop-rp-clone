(define (problem fix4)
  (:domain init-flat-tire)
  (:objects wheel1 wheel2 hub nuts boot)
  (:init (wheel wheel1)
	 (wheel wheel2) (hub hub) (nut nuts)
	 (container boot) (intact wheel2)
	 (have jack) (have pump) (have wheel1)
	 (have wrench) (open boot)
	 (inflated wheel2)
	 (on wheel2 hub)
	 (tight nuts hub) (on-ground hub))
  (:goal (and (in jack boot) (in pump boot) (in wheel1 boot)
	      (in wrench boot) (inflated wheel2) (on wheel2 hub)
	      (tight nuts hub))))
