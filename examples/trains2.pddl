(define (problem trains2)
  (:domain trains-domain)
  (:requirements :existential-preconditions)
  (:objects avon bath corning dansville elmira
	    tr1 tr2 tr3 tr4 tr5
	    e1 e2 e3
	    bc1 bc2 bc3 bc4 tc1
	    ors1 bas1 oj-fac1)
  (:init (city avon) (city bath) (city corning) (city dansville) (city elmira)
	 (track tr1) (track tr2) (track tr3) (track tr4) (track tr5)
	 (connects tr1 avon bath) (connects tr1 bath avon)
	 (connects tr2 bath corning) (connects tr4 corning dansville)
	 (connects tr3 avon dansville) (connects tr3 dansville avon)
	 (connects tr5 corning elmira) (connects tr5 elmira corning)
	 (engine e1) (engine e2) (engine e3)
	 (car bc1) (car bc2) (car bc3) (car bc4) (car tc1)
	 (boxcar bc1) (boxcar bc2) (boxcar bc3) (boxcar bc4) (tanker-car tc1)
	 (oranges ors1) (bananas bas1) (oj-fac oj-fac1)
	 (empty bc1) (empty bc2) (empty bc3) (empty bc4) (empty tc1)
	 (loose bc1) (loose bc2) (loose bc3) (loose bc4) (loose tc1)
	 (at e1 avon) (at bas1 avon) (at bc1 bath) (at bc2 bath)
	 (at bc3 dansville) (at tc1 corning) (at ors1 elmira)
	 (at e2 elmira) (at e3 corning) (at bc4 elmira) (at oj-fac1 corning))
  (:goal (exists (?x) (and (oj ?x) (at ?x dansville)))))
