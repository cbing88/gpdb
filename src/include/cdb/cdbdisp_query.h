/*-------------------------------------------------------------------------
 *
 * cdbdisp_query.h
 * routines for dispatching command string or plan to the qExec processes.
 *
 * Copyright (c) 2005-2008, Greenplum inc
 *
 *-------------------------------------------------------------------------
 */
#ifndef CDBDISP_QUERY_H
#define CDBDISP_QUERY_H

#include "lib/stringinfo.h" /* StringInfo */
#include "cdb/cdbtm.h"

#define EUS_NONE 0x0
#define EUS_CANCEL_ON_ERROR 0x1
#define EUS_NEED_TWO_PHASE 0x2
#define EUS_WITH_SNAPSHOT  0x4

struct QueryDesc;
struct CdbDispatcherState;
struct CdbPgResults;

/* Compose and dispatch the MPPEXEC commands corresponding to a plan tree
 * within a complete parallel plan.
 *
 * The CdbDispatchResults objects allocated for the plan are
 * returned in *pPrimaryResults
 * The caller, after calling CdbCheckDispatchResult(), can
 * examine the CdbDispatchResults objects, can keep them as
 * long as needed, and ultimately must free them with
 * cdbdisp_destroyDispatchState() prior to deallocation
 * of the caller's memory context.
 *
 * NB: Callers should use PG_TRY()/PG_CATCH() if needed to make
 * certain that the CdbDispatchResults objects are destroyed by
 * cdbdisp_destroyDispatchState() in case of error.
 * To wait for completion, check for errors, and clean up, it is
 * suggested that the caller use cdbdisp_finishCommand().
 */
void
cdbdisp_dispatchPlan(struct QueryDesc *queryDesc,
					 bool planRequiresTxn,
					 bool cancelOnError,
					 struct CdbDispatcherState *ds);

/*
 * Special for sending SET commands that change GUC variables, so they go to all
 * gangs, both reader and writer
 */
void
CdbSetGucOnAllGangs(const char *strCommand, bool cancelOnError, bool needTwoPhase);

/*
 * CdbDispatchCommand
 *
 * Dispatch an plain command to all primary writer QEs, wait until
 * all QEs finished successfully. If one or more QEs got error,
 * throw an Error.
 *
 * -flags:
 * 	Is the combination of EUS_NEED_TWO_PHASE, EUS_WITH_SNAPSHOT,EUS_CANCEL_ON_ERROR
 *
 * -returnPgResults:
 * 	Indicate whether return the pg_result for each QE connection.
 *
 * If returnPgResults is true, caller need to call cdbdisp_freeCdbPgResults() to
 * clear pg_results.
 */
struct CdbPgResults*
CdbDispatchCommand(const char* strCommand,
					int flags,
					bool returnPgResults);

/*
 * CdbDispatchUtilityStatement
 *
 * Dispatch an already parsed statement to all primary writer QEs, wait until
 * all QEs finished successfully. If one or more QEs got error,
 * throw an Error.
 *
 * -flags:
 * 	Is the combination of EUS_NEED_TWO_PHASE, EUS_WITH_SNAPSHOT,EUS_CANCEL_ON_ERROR
 *
 * -returnPgResults:
 * 	Indicate whether return the pg_result for each QE connection.
 *
 * If returnPgResults is true, caller need to call cdbdisp_freeCdbPgResults() to
 * clear pg_results.
 */
struct CdbPgResults *
CdbDispatchUtilityStatement(struct Node *stmt,
							int flag,
							bool returnPgResults);


#endif   /* CDBDISP_QUERY_H */
