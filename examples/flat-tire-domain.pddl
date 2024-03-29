; The flat-tire domain from the UCPOP distribution.

(define (domain init-flat-tire)
  (:requirements :equality :negative-preconditions)
  (:constants wrench jack pump)
  (:predicates (annoyed)
	       (container ?x)
	       (locked ?x)
	       (open ?x)
	       (have ?x)
	       (in ?x ?y)
	       (hub ?x)
	       (nut ?x)
	       (tight ?x ?y)
	       (on-ground ?x)
	       (loose ?x ?y)
	       (unfastened ?x)
	       (on ?x ?y)
	       (wheel ?x)
	       (free ?x)
	       (inflated ?x)
	       (intact ?x))
  (:action cuss
	   :parameters ()
	   :effect (not (annoyed)))
  (:action open
	   :parameters (?x)
	   :precondition (and (container ?x)
			      (not (locked ?x)) (not (open ?x)))
	   :effect (open ?x))
  (:action close
	   :parameters (?x)
	   :precondition (and (container ?x)
			      (open ?x))
	   :effect (not (open ?x)))
  (:action fetch
	   :parameters (?x ?y)
	   :precondition (and (container ?y)
			      (not (= ?x ?y)) (in ?x ?y) (open ?y))
	   :effect (and (have ?x)
			(not (in ?x ?y))))
  (:action put-away
	   :parameters (?x ?y)
	   :precondition (and (container ?y)
			      (not (= ?x ?y)) (have ?x) (open ?y))
	   :effect (and (in ?x ?y)
			(not (have ?x))))
  (:action loosen
	   :parameters (?x ?y)
	   :precondition (and (nut ?x) (hub ?y)
			      (not (= ?x ?y)) (have wrench) (tight ?x ?y)
			      (on-ground ?y))
	   :effect (and (loose ?x ?y)
			(not (tight ?x ?y))))
  (:action tighten
	   :parameters (?x ?y)
	   :precondition (and (nut ?x) (hub ?y)
			      (not (= ?x ?y)) (have wrench) (loose ?x ?y)
			      (on-ground ?y))
	   :effect (and (tight ?x ?y)
			(not (loose ?x ?y))))
  (:action jack-up
	   :parameters (?y)
	   :precondition (and (hub ?y)
			      (on-ground ?y) (have jack))
	   :effect (and (not (on-ground ?y))
			(not (have jack))))
  (:action jack-down
	   :parameters (?x)
	   :precondition (and (hub ?x)
			      (not (on-ground ?x)))
	   :effect (and (on-ground ?x)
			(have jack)))
  (:action undo
	   :parameters (?x ?y)
	   :precondition (and (nut ?x) (hub ?y)
			      (not (= ?x ?y))
			      (not (on-ground ?y)) (not (unfastened ?y))
			      (have wrench) (loose ?x ?y))
	   :effect (and (have ?x) (unfastened ?y)
			(not (on ?x ?y)) (not (loose ?x ?y))))
  (:action do-up
	   :parameters (?x ?y)
	   :precondition (and (nut ?x) (hub ?y)
			      (not (= ?x ?y))
			      (have wrench) (unfastened ?y)
			      (not (on-ground ?y)) (have ?x))
	   :effect (and (loose ?x ?y) (not (unfastened ?y)) (not (have ?x))))
  (:action remove-wheel
	   :parameters (?x ?y)
	   :precondition (and (wheel ?x) (hub ?y)
			      (not (= ?x ?y)) (not (on-ground ?y))
			      (on ?x ?y) (unfastened ?y))
	   :effect (and (have ?x) (free ?y) (not (on ?x ?y))))
  (:action put-on-wheel
	   :parameters (?x ?y)
	   :precondition (and (wheel ?x) (hub ?y)
			      (not (= ?x ?y)) (have ?x) (free ?y)
			      (unfastened ?y) (not (on-ground ?y)))
	   :effect (and (on ?x ?y) (not (have ?x)) (not (free ?y))))
  (:action inflate
	   :parameters (?x)
	   :precondition (and (wheel ?x)
			      (have pump) (not (inflated ?x)) (intact ?x))
	   :effect (inflated ?x)))
