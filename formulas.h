/* -*-C++-*- */
/*
 * Formulas.
 *
 * Copyright (C) 2003 Carnegie Mellon University
 * Written by H�kan L. S. Younes.
 *
 * Permission is hereby granted to distribute this software for
 * non-commercial research purposes, provided that this copyright
 * notice is included with any such distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
 * SOFTWARE IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU
 * ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
 *
 * $Id: formulas.h,v 4.11 2003/03/01 18:52:17 lorens Exp $
 */
#ifndef FORMULAS_H
#define FORMULAS_H

#include <config.h>
#include "hashing.h"
#include <string>
#include <vector>
#include <map>

#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

struct Type;
struct Predicate;
struct Problem;
struct Bindings;
struct HeuristicValue;
struct PlanningGraph;


/* ====================================================================== */
/* Substitution */

struct Term;
struct Variable;

/*
 * Variable substitution.
 */
struct Substitution {
  /* Constructs a substitution. */
  Substitution(const Variable& var, const Term& term)
    : var_(&var), var_id_(0), term_(&term), term_id_(0) {}

  /* Constructs a substitution with assigned step ids. */
  Substitution(const Variable& var, size_t var_id,
	       const Term& term, size_t term_id)
    : var_(&var), var_id_(var_id), term_(&term), term_id_(term_id) {}

  /* Returns the variable to get substituted. */
  const Variable& var() const { return *var_; }

  /* Returns the step id of the variable. */
  size_t var_id() const { return var_id_; }

  /* Returns the term to substitute with. */
  const Term& term() const { return *term_; }

  /* Returns the step id of the term. */
  size_t term_id() const { return term_id_; }

private:
  /* Variable to get substituted. */
  const Variable* var_;
  /* Step id of variable. */
  size_t var_id_;
  /* Term to substitute with. */
  const Term* term_;
  /* Step id of term. */
  size_t term_id_;
};


/* ====================================================================== */
/* SubstitutionList */

/*
 * List of substitutions.
 */
struct SubstitutionList : public std::vector<Substitution> {
};

/* Substitution list iterator. */
typedef SubstitutionList::const_iterator SubstListIter;


/* ====================================================================== */
/* Term */

/*
 * Abstract term.
 */
struct Term {
  /* Deletes this term. */
  virtual ~Term();

  /* Adds the given type to the type of this term. */
  void add_type(const Type& type);

  /* Returns the name of this term. */
  const std::string& name() const { return name_; }

  /* Returns the type of this term. */
  const Type& type() const { return *type_; }

  /* Returns this term subject to the given substitutions. */
  virtual const Term& substitution(const SubstitutionList& subst,
				   size_t step_id) const = 0;

  /* Prints this term on the given stream with the given bindings. */
  void print(std::ostream& os, size_t step_id, const Bindings& bindings) const;

protected:
  /* Constructs an abstract term with the given name. */
  Term(const std::string& name, const Type& type);

private:
  /* Name of term. */
  std::string name_;
  /* Type of term. */
  const Type* type_;
};

/* Equality operator for terms. */
inline bool operator==(const Term& t1, const Term& t2) {
  return &t1 == &t2;
}

/* Inequality operator for terms. */
inline bool operator!=(const Term& t1, const Term& t2) {
  return &t1 != &t2;
}

/* Output operator for terms. */
std::ostream& operator<<(std::ostream& os, const Term& t);


/* ====================================================================== */
/* Name */

/*
 * Name.
 */
struct Name : public Term {
  /* Constructs a name. */
  Name(const std::string& name, const Type& type);

  /* Returns this term subject to the given substitutions. */
  virtual const Name& substitution(const SubstitutionList& subst,
				   size_t step_id) const;
};


/* ====================================================================== */
/* Variable */

/*
 * Variable.
 */
struct Variable : public Term {
  /* Register use of the given variable. */
  static void register_use(const Variable* v) {
    if (v != NULL) {
      v->ref_count_++;
    }
  }

  /* Unregister use of the given variable. */
  static void unregister_use(const Variable* v) {
    if (v != NULL) {
      v->ref_count_--;
      if (v->ref_count_ == 0) {
	delete v;
      }
    }
  }

  /* Constructs a variable with the given name and type. */
  Variable(const std::string& name, const Type& type);

  /* Deletes this variable. */
  virtual ~Variable();

  /* Returns this term subject to the given substitutions. */
  virtual const Term& substitution(const SubstitutionList& subst,
				   size_t step_id) const;

private:
  /* Reference counter. */
  mutable size_t ref_count_;
};


/* ====================================================================== */
/* TermList */

/*
 * List of terms.
 */
struct TermList : public std::vector<const Term*> {
};

/* Term list iterator. */
typedef TermList::const_iterator TermListIter;


/* ====================================================================== */
/* NameList */

/*
 * List of names.
 */
struct NameList : public std::vector<const Name*> {
};

/* Name list iterator. */
typedef NameList::const_iterator NameListIter;


/* ====================================================================== */
/* NameMap */

/*
 * Table of names.
 */
struct NameMap : public std::map<std::string, Name*> {
};

/* Name table iterator. */
typedef NameMap::const_iterator NameMapIter;


/* ====================================================================== */
/* VariableList */

/*
 * List of variables.
 */
struct VariableList : public std::vector<const Variable*> {
};

/* Variable list iterator. */
typedef VariableList::const_iterator VarListIter;


/* ====================================================================== */
/* Formula */

struct Literal;

/*
 * Abstract formula.
 */
struct Formula {
  /* Possible temporal annotations for formulas. */
  typedef enum { AT_START, AT_END, OVER_ALL } FormulaTime;

  /* The true formula. */
  static const Formula& TRUE;
  /* The false formula. */
  static const Formula& FALSE;

  /* Register use of the given formula. */
  static void register_use(const Formula* f) {
    if (f != NULL) {
      f->ref_count_++;
    }
  }

  /* Unregister use of the given formula. */
  static void unregister_use(const Formula* f) {
    if (f != NULL) {
      f->ref_count_--;
      if (f->ref_count_ == 0) {
	delete f;
      }
    }
  }

  /* Deletes this formula. */
  virtual ~Formula();

  /* Checks if this formula is a tautology. */
  bool tautology() const { return this == &TRUE; }

  /* Checks if this formula is a contradiction. */
  bool contradiction() const { return this == &FALSE; }

  /* Checks if this formula is either a tautology or contradiction. */
  bool constant() const { return tautology() || contradiction(); }

  /* Returns a formula that separates the given literal from anything
     definitely asserted by this formula. */
  virtual const Formula& separate(const Literal& literal) const = 0;

  /* Returns an instantiation of this formula. */
  virtual const Formula& instantiation(const SubstitutionList& subst,
				       const Problem& problem) const = 0;

  /* Returns this formula subject to the given substitutions. */
  virtual const Formula& substitution(const SubstitutionList& subst,
				      size_t step_id) const = 0;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, const PlanningGraph& pg,
			       size_t step_id,
			       const Bindings* b = NULL) const = 0;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, HeuristicValue& hs,
			       const PlanningGraph& pg, size_t step_id,
			       const Bindings* b = NULL) const = 0;

  /* Prints this formula on the given stream with the given bindings. */
  virtual void print(std::ostream& os, size_t step_id,
		     const Bindings& bindings) const = 0;

protected:
  /* Constructs a formula. */
  Formula();

  /* Returns the negation of this formula. */
  virtual const Formula& negation() const = 0;

  /* Prints this object on the given stream. */
  virtual void print(std::ostream& os) const = 0;

private:
  mutable size_t ref_count_;

  friend const Formula& operator!(const Formula& f);
  friend std::ostream& operator<<(std::ostream& os, const Formula& f);
};

/* Negation operator for formulas. */
const Formula& operator!(const Formula& f);

/* Conjunction operator for formulas. */
const Formula& operator&&(const Formula& f1, const Formula& f2);

/* Disjunction operator for formulas. */
const Formula& operator||(const Formula& f1, const Formula& f2);

/* Output operator for formulas. */
std::ostream& operator<<(std::ostream& os, const Formula& f);


/* ====================================================================== */
/* FormulaList */

/*
 * List of formulas.
 */
struct FormulaList : public std::vector<const Formula*> {
};

/* A formula list const iterator. */
typedef FormulaList::const_iterator FormulaListIter;


/* ====================================================================== */
/* Constant */

/*
 * A formula with a constant truth value.
 */
struct Constant : public Formula {
  /* Returns a formula that separates the given literal from anything
     definitely asserted by this formula. */
  virtual const Formula& separate(const Literal& literal) const;

  /* Returns an instantiation of this formula. */
  virtual const Constant& instantiation(const SubstitutionList& subst,
					const Problem& problem) const;

  /* Returns this formula subject to the given substitutions. */
  virtual const Constant& substitution(const SubstitutionList& subst,
				       size_t step_id) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, const PlanningGraph& pg,
			       size_t step_id, const Bindings* b) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, HeuristicValue& hs,
			       const PlanningGraph& pg, size_t step_id,
			       const Bindings* b) const;

  /* Prints this formula on the given stream with the given bindings. */
  virtual void print(std::ostream& os, size_t step_id,
		     const Bindings& bindings) const;

protected:
  /* Returns a negation of this formula. */
  virtual const Formula& negation() const;

  /* Prints this object on the given stream. */
  virtual void print(std::ostream& os) const;

private:
  /* Constant representing true. */
  static const Constant TRUE_;
  /* Constant representing false. */
  static const Constant FALSE_;

  /* Value of this constant. */
  bool value_;

  /* Constructs a constant formula. */
  Constant(bool value);

  friend struct Formula;
};


/* ====================================================================== */
/* Literal */

/*
 * An abstract literal.
 */
struct Literal : public Formula {
  /* Returns the temporal annotation for this formula. */
  FormulaTime when() const { return when_; }

  /* Returns the predicate of this literal. */
  virtual const Predicate& predicate() const = 0;

  /* Returns the terms of this literal. */
  virtual const TermList& terms() const = 0;

  /* Returns a formula that separates the given literal from anything
     definitely asserted by this formula. */
  virtual const Formula& separate(const Literal& literal) const;

protected:
  /* Constructs a literal. */
  explicit Literal(FormulaTime when);

  /* Checks if this object equals the given object. */
  virtual bool equals(const Literal& o) const = 0;

  /* Returns the hash value of this object. */
  virtual size_t hash_value() const = 0;

private:
  /* Temporal annotation for this formula. */
  FormulaTime when_;

  friend bool operator==(const Literal& l1, const Literal& l2);
  friend struct hashing::hash<const Literal*>;
};

/* Equality operator for literals. */
inline bool operator==(const Literal& l1, const Literal& l2) {
  return l1.equals(l2);
}

/* Inequality operator for literals. */
inline bool operator!=(const Literal& l1, const Literal& l2) {
  return !(l1 == l2);
}

/*
 * Equality function object for literal pointers.
 */
namespace std {
struct equal_to<const Literal*>
  : public binary_function<const Literal*, const Literal*, bool> {
  bool operator()(const Literal* l1, const Literal* l2) const {
    return *l1 == *l2;
  }
};
}

/*
 * Hash function object for literal pointers.
 */
#if defined __GNUC__ && __GNUC__ >= 3
# if __GNUC_MINOR__ == 0
namespace std
# else
namespace __gnu_cxx
# endif
{
#endif
  struct hash<const Literal*> {
    size_t operator()(const Literal* l) const {
      return l->hash_value();
    }
  };
#if defined __GNUC__ && __GNUC__ >= 3
}
#endif


/* ====================================================================== */
/* Atom */

/*
 * An atom.
 */
struct Atom : public Literal {
  /* Constructs an atomic formula with the given predicate. */
  Atom(const Predicate& predicate, FormulaTime when);

  /* Adds a term to this atomic formula. */
  void add_term(const Term& term);

  /* Returns the predicate of this literal. */
  virtual const Predicate& predicate() const { return *predicate_; }

  /* Returns the terms of this literal. */
  virtual const TermList& terms() const { return terms_; }

  /* Returns an instantiation of this formula. */
  virtual const Formula& instantiation(const SubstitutionList& subst,
				       const Problem& problem) const;

  /* Returns this formula subject to the given substitutions. */
  virtual const Atom& substitution(const SubstitutionList& subst,
				   size_t step_id) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, const PlanningGraph& pg,
			       size_t step_id, const Bindings* b) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, HeuristicValue& hs,
			       const PlanningGraph& pg, size_t step_id,
			       const Bindings* b) const;

  /* Prints this formula on the given stream with the given bindings. */
  virtual void print(std::ostream& os, size_t step_id,
		     const Bindings& bindings) const;

protected:
  /* Checks if this object equals the given object. */
  virtual bool equals(const Literal& o) const;

  /* Returns the hash value of this object. */
  virtual size_t hash_value() const;

  /* Returns the negation of this formula. */
  virtual const Literal& negation() const;

  /* Prints this object on the given stream. */
  virtual void print(std::ostream& os) const;

private:
  /* Predicate of this atom. */
  const Predicate* predicate_;
  /* Terms of this atom. */
  TermList terms_;
};


/* ====================================================================== */
/* Negation */

/*
 * A negated atom.
 */
struct Negation : public Literal {
  /* Constructs a negated atom. */
  explicit Negation(const Atom& atom);

  /* Deletes this negated atom. */
  virtual ~Negation();

  /* Returns the the negated atom. */
  const Atom& atom() const { return *atom_; }

  /* Returns the predicate of this literal. */
  virtual const Predicate& predicate() const { return atom().predicate(); }

  /* Returns the terms of this literal. */
  virtual const TermList& terms() const { return atom().terms(); }

  /* Returns an instantiation of this formula. */
  virtual const Formula& instantiation(const SubstitutionList& subst,
				       const Problem& problem) const;

  /* Returns this formula subject to the given substitutions. */
  virtual const Negation& substitution(const SubstitutionList& subst,
				       size_t step_id) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, const PlanningGraph& pg,
			       size_t step_id, const Bindings* b) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, HeuristicValue& hs,
			       const PlanningGraph& pg, size_t step_id,
			       const Bindings* b) const;

  /* Prints this formula on the given stream with the given bindings. */
  virtual void print(std::ostream& os, size_t step_id,
		     const Bindings& bindings) const;

protected:
  /* Checks if this object equals the given object. */
  virtual bool equals(const Literal& o) const;

  /* Returns the hash value of this object. */
  virtual size_t hash_value() const;

  /* Returns the negation of this formula. */
  virtual const Literal& negation() const;

  /* Prints this object on the given stream. */
  virtual void print(std::ostream& os) const;

private:
  /* The negated atom. */
  const Atom* atom_;
};


/* ====================================================================== */
/* BindingLiteral */

/*
 * A binding literal.
 */
struct BindingLiteral : public Formula {
  /* Returns the first term of binding literal. */
  const Term& term1() const { return *term1_; }

  /* Returns the step id of the first term, or the given id if no step
     id has been assigned. */
  size_t step_id1(size_t def_id) const { return (id1_ != 0) ? id1_ : def_id; }

  /* Returns the second term of binding literal. */
  const Term& term2() const { return *term2_; }

  /* Returns the step id of the second term, or the given id if no
     step id has been assigned. */
  size_t step_id2(size_t def_id) const { return (id2_ != 0) ? id2_ : def_id; }

  /* Returns a formula that separates the given literal from anything
     definitely asserted by this formula. */
  virtual const Formula& separate(const Literal& literal) const;

protected:
  /* Constructs a binding literal. */
  BindingLiteral(const Term& term1, size_t id1, const Term& term2, size_t id2);

private:
  /* First term of binding literal. */
  const Term* term1_;
  /* Step id of first term, or zero if unassigned. */
  size_t id1_;
  /* Second term of binding literal. */
  const Term* term2_;
  /* Step id of second term, or zero if unassigned. */
  size_t id2_;
};


/* ====================================================================== */
/* Equality */

/*
 * Equality formula.
 */
struct Equality : public BindingLiteral {
  /* Constructs an equality. */
  Equality(const Term& term1, const Term& term2);

  /* Constructs an equality with assigned step ids. */
  Equality(const Term& term1, size_t id1, const Term& term2, size_t id2);

  /* Returns an instantiation of this formula. */
  virtual const Formula& instantiation(const SubstitutionList& subst,
				       const Problem& problem) const;

  /* Returns this formula subject to the given substitutions. */
  virtual const Formula& substitution(const SubstitutionList& subst,
				      size_t step_id) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, const PlanningGraph& pg,
			       size_t step_id, const Bindings* b) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, HeuristicValue& hs,
			       const PlanningGraph& pg, size_t step_id,
			       const Bindings* b) const;

  /* Prints this formula on the given stream with the given bindings. */
  virtual void print(std::ostream& os, size_t step_id,
		     const Bindings& bindings) const;

protected:
  /* Returns the negation of this formula. */
  virtual const BindingLiteral& negation() const;

  /* Prints this object on the given stream. */
  virtual void print(std::ostream& os) const;
};


/* ====================================================================== */
/* Inequality */

/*
 * Inequality formula.
 */
struct Inequality : public BindingLiteral {
  /* Constructs an inequality. */
  Inequality(const Term& term1, const Term& term2);

  /* Constructs an inequality with assigned step ids. */
  Inequality(const Term& term1, size_t id1, const Term& term2, size_t id2);

  /* Returns an instantiation of this formula. */
  virtual const Formula& instantiation(const SubstitutionList& subst,
				       const Problem& problem) const;

  /* Returns this formula subject to the given substitutions. */
  virtual const Formula& substitution(const SubstitutionList& subst,
				      size_t step_id) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, const PlanningGraph& pg,
			       size_t step_id, const Bindings* b) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, HeuristicValue& hs,
			       const PlanningGraph& pg, size_t step_id,
			       const Bindings* b) const;

  /* Prints this formula on the given stream with the given bindings. */
  virtual void print(std::ostream& os, size_t step_id,
		     const Bindings& bindings) const;

protected:
  /* Returns the negation of this formula. */
  virtual const BindingLiteral& negation() const;

  /* Prints this object on the given stream. */
  virtual void print(std::ostream& os) const;
};


/* ====================================================================== */
/* Conjunction */

/*
 * Conjunction.
 */
struct Conjunction : public Formula {
  /* Constructs an empty conjunction. */
  Conjunction();

  /* Deletes this conjunction. */
  virtual ~Conjunction();

  /* Adds a conjunct to this conjunction. */
  void add_conjunct(const Formula& conjunct);

  /* Returns the conjuncts. */
  const FormulaList& conjuncts() const { return conjuncts_; }

  /* Returns a formula that separates the given literal from anything
     definitely asserted by this formula. */
  virtual const Formula& separate(const Literal& literal) const;

  /* Returns an instantiation of this formula. */
  virtual const Formula& instantiation(const SubstitutionList& subst,
				       const Problem& problem) const;

  /* Returns this formula subject to the given substitutions. */
  virtual const Formula& substitution(const SubstitutionList& subst,
				      size_t step_id) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, const PlanningGraph& pg,
			       size_t step_id, const Bindings* b) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, HeuristicValue& hs,
			       const PlanningGraph& pg, size_t step_id,
			       const Bindings* b) const;

  /* Prints this formula on the given stream with the given bindings. */
  virtual void print(std::ostream& os, size_t step_id,
		     const Bindings& bindings) const;

protected:
  /* Returns the negation of this formula. */
  virtual const Formula& negation() const;

  /* Prints this object on the given stream. */
  virtual void print(std::ostream& os) const;

private:
  /* The conjuncts. */
  FormulaList conjuncts_;
};


/* ====================================================================== */
/* Disjunction */

/*
 * Disjunction.
 */
struct Disjunction : public Formula {
  /* Constructs an empty disjunction. */
  Disjunction();

  /* Deletes this disjunction. */
  virtual ~Disjunction();

  /* Adds a disjunct to this disjunction. */
  void add_disjunct(const Formula& disjunct);

  /* Returns the disjuncts. */
  const FormulaList& disjuncts() const { return disjuncts_; }

  /* Returns a formula that separates the given literal from anything
     definitely asserted by this formula. */
  virtual const Formula& separate(const Literal& literal) const;

  /* Returns an instantiation of this formula. */
  virtual const Formula& instantiation(const SubstitutionList& subst,
				       const Problem& problem) const;

  /* Returns this formula subject to the given substitution. */
  virtual const Formula& substitution(const SubstitutionList& subst,
				      size_t step_id) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, const PlanningGraph& pg,
			       size_t step_id, const Bindings* b) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, HeuristicValue& hs,
			       const PlanningGraph& pg, size_t step_id,
			       const Bindings* b) const;

  /* Prints this formula on the given stream with the given bindings. */
  virtual void print(std::ostream& os, size_t step_id,
		     const Bindings& bindings) const;

protected:
  /* Returns the negation of this formula. */
  virtual const Formula& negation() const;

  /* Prints this object on the given stream. */
  virtual void print(std::ostream& os) const;

private:
  /* The disjuncts. */
  FormulaList disjuncts_;
};


/* ====================================================================== */
/* QuantifiedFormula */

/*
 * Abstract quantified formula.
 */
struct QuantifiedFormula : public Formula {
  /* Deletes this quantified formula. */
  virtual ~QuantifiedFormula();

  /* Adds a quantified variable to this quantified formula. */
  void add_parameter(const Variable& parameter);

  /* Sets the body of this quantified formula. */
  void set_body(const Formula& body);

  /* Returns the quanitfied variables. */
  const VariableList& parameters() const { return parameters_; }

  /* Returns the quantified formula. */
  const Formula& body() const { return *body_; }

  /* Returns a formula that separates the given literal from anything
     definitely asserted by this formula. */
  virtual const Formula& separate(const Literal& literal) const;

protected:
  /* Constructs a quantified formula. */
  explicit QuantifiedFormula(const Formula& body);

private:
  /* Quanitfied variables. */
  VariableList parameters_;
  /* The quantified formula. */
  const Formula* body_;
};


/* ====================================================================== */
/* ExistsFormula */

/*
 * Existentially quantified formula.
 */
struct ExistsFormula : public QuantifiedFormula {
  /* Constructs an existentially quantified formula. */
  ExistsFormula();

  /* Returns an instantiation of this formula. */
  virtual const Formula& instantiation(const SubstitutionList& subst,
				       const Problem& problem) const;

  /* Returns this formula subject to the given substitutions. */
  virtual const Formula& substitution(const SubstitutionList& subst,
				      size_t step_id) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, const PlanningGraph& pg,
			       size_t step_id, const Bindings* b) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, HeuristicValue& hs,
			       const PlanningGraph& pg, size_t step_id,
			       const Bindings* b) const;

  /* Prints this formula on the given stream with the given bindings. */
  virtual void print(std::ostream& os, size_t step_id,
		     const Bindings& bindings) const;

protected:
  /* Returns the negation of this formula. */
  virtual const QuantifiedFormula& negation() const;

  /* Prints this object on the given stream. */
  virtual void print(std::ostream& os) const;
};


/* ====================================================================== */
/* ForallFormula */

/*
 * Universally quantified formula.
 */
struct ForallFormula : public QuantifiedFormula {
  /* Constructs a universally quantified formula. */
  ForallFormula();

  /* Returns an instantiation of this formula. */
  virtual const Formula& instantiation(const SubstitutionList& subst,
				       const Problem& problem) const;

  /* Returns this formula subject to the given substitutions. */
  virtual const Formula& substitution(const SubstitutionList& subst,
				      size_t step_id) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, const PlanningGraph& pg,
			       size_t step_id, const Bindings* b) const;

  /* Returns the heuristic value of this formula. */
  virtual void heuristic_value(HeuristicValue& h, HeuristicValue& hs,
			       const PlanningGraph& pg, size_t step_id,
			       const Bindings* b) const;

  /* Prints this formula on the given stream with the given bindings. */
  virtual void print(std::ostream& os, size_t step_id,
		     const Bindings& bindings) const;

protected:
  /* Returns the negation of this formula. */
  virtual const QuantifiedFormula& negation() const;

  /* Prints this object on the given stream. */
  virtual void print(std::ostream& os) const;
};


/* ====================================================================== */
/* AtomList */

/*
 * List of atoms.
 */
struct AtomList : public std::vector<const Atom*> {
};

/* Atom list iterator. */
typedef AtomList::const_iterator AtomListIter;


/* ====================================================================== */
/* NegationList */

/*
 * List of negated atoms.
 */
struct NegationList : public std::vector<const Negation*> {
};

/* Negation list iterator */
typedef NegationList::const_iterator NegationListIter;


#endif /* FORMULAS_H */
