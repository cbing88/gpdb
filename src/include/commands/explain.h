/*-------------------------------------------------------------------------
 *
 * explain.h
 *	  prototypes for explain.c
 *
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994-5, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/commands/explain.h,v 1.29 2007/01/05 22:19:53 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef EXPLAIN_H
#define EXPLAIN_H

#include "executor/executor.h"


extern void ExplainQuery(ExplainStmt *stmt, const char *queryString,
						 ParamListInfo params, DestReceiver *dest);

extern TupleDesc ExplainResultDesc(ExplainStmt *stmt);

extern void ExplainOneUtility(Node *utilityStmt, ExplainStmt *stmt,
				  const char *queryString,
				  ParamListInfo params,
				  TupOutputState *tstate);

extern void ExplainOnePlan(PlannedStmt *plannedstmt, ExplainStmt *stmt,
		   const char *queryString, ParamListInfo params,
		   TupOutputState *tstate);

#endif   /* EXPLAIN_H */
