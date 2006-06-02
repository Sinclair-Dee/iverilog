#ifndef __netmisc_H
#define __netmisc_H
/*
 * Copyright (c) 1999-2000 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#ifdef HAVE_CVS_IDENT
#ident "$Id: netmisc.h,v 1.25 2006/06/02 04:48:50 steve Exp $"
#endif

# include  "netlist.h"

/*
 * Search for a symbol using the "start" scope as the starting
 * point. If the path includes a scope part, then locate the
 * scope first.
 *
 * The return value is the scope where the symbol was found.
 * If the symbol was not found, return 0. The output arguments
 * get 0 except for the pointer to the object that represents
 * the located symbol.
 *
 * The ex1 and ex2 output arguments are extended results. If the
 * symbol is a parameter (par!=0) then ex1 is the msb expression and
 * ex2 is the lsb expression for the range. If there is no range, then
 * these values are set to 0.
 */
extern NetScope* symbol_search(const Design*des,
			       NetScope*start, hname_t path,
			       NetNet*&net,       /* net/reg */
			       NetMemory*&mem,    /* memory */
			       const NetExpr*&par,/* parameter */
			       NetEvent*&eve,     /* named event */
			       const NetExpr*&ex1, const NetExpr*&ex2);

inline NetScope* symbol_search(const Design*des,
			       NetScope*start, const hname_t&path,
			       NetNet*&net,       /* net/reg */
			       NetMemory*&mem,    /* memory */
			       const NetExpr*&par,/* parameter */
			       NetEvent*&eve      /* named event */)
{
      const NetExpr*ex1, *ex2;
      return symbol_search(des, start, path, net, mem, par, eve, ex1, ex2);
}

/*
 * This function transforms an expression by padding the high bits
 * with V0 until the expression has the desired width. This may mean
 * not transforming the expression at all, if it is already wide
 * enough.
 */
extern NetExpr*pad_to_width(NetExpr*expr, unsigned wid);
extern NetNet*pad_to_width(Design*des, NetNet*n, unsigned w);

extern NetNet*pad_to_width_signed(Design*des, NetNet*n, unsigned w);

/*
 * This function transforms an expression by cropping the high bits
 * off with a part select. The result has the width w passed in. This
 * function does not pad, use pad_to_width if padding is desired.
 */
extern NetNet*crop_to_width(Design*des, NetNet*n, unsigned w);

/*
 * This function takes as input a NetNet signal and adds a constant
 * value to it. If the val is 0, then simply return sig. Otherwise,
 * return a new NetNet value that is the output of an addition.
 */
extern NetNet*add_to_net(Design*des, NetNet*sig, long val);

/*
 * These functions make various sorts of expressions, given operands
 * of certain type. The order of the operands is preserved in cases
 * where order matters.
 *
 * make_add_expr
 *   Make a NetEBAdd expresion with <expr> the first argument and
 *   <val> the second. This may get turned into a subtract if <val> is
 *   less then zero. If val is exactly zero, then return <expr> as is.
 *
 * make_sub_expr
 *   Make a NetEBAdd(subtract) node that subtracts the given
 *   expression from the integer value.
 */
extern NetExpr*make_add_expr(NetExpr*expr, long val);
extern NetExpr*make_sub_expr(long val, NetExpr*expr);

/*
 * In some cases the lval is accessible as a pointer to the head of
 * a list of NetAssign_ objects. This function returns the width of
 * the l-value represented by this list.
 */
extern unsigned count_lval_width(const class NetAssign_*first);

/*
 * This function elaborates an expression, and tries to evaluate it
 * right away. If the expression can be evaluated, this returns a
 * constant expression. If it cannot be evaluated, it returns whatever
 * it can. If the expression cannot be elaborated, return 0.
 */
class PExpr;
extern NetExpr* elab_and_eval(Design*des, NetScope*scope,
			      const PExpr*pe, int expr_wid);

/*
 * $Log: netmisc.h,v $
 * Revision 1.25  2006/06/02 04:48:50  steve
 *  Make elaborate_expr methods aware of the width that the context
 *  requires of it. In the process, fix sizing of the width of unary
 *  minus is context determined sizes.
 *
 * Revision 1.24  2005/11/27 05:56:20  steve
 *  Handle bit select of parameter with ranges.
 *
 * Revision 1.23  2005/07/11 16:56:51  steve
 *  Remove NetVariable and ivl_variable_t structures.
 *
 * Revision 1.22  2005/05/24 01:44:28  steve
 *  Do sign extension of structuran nets.
 *
 * Revision 1.21  2005/04/24 23:44:02  steve
 *  Update DFF support to new data flow.
 *
 * Revision 1.20  2005/01/24 05:28:31  steve
 *  Remove the NetEBitSel and combine all bit/part select
 *  behavior into the NetESelect node and IVL_EX_SELECT
 *  ivl_target expression type.
 *
 * Revision 1.19  2004/03/07 20:04:11  steve
 *  MOre thorough use of elab_and_eval function.
 *
 * Revision 1.18  2003/09/19 03:30:05  steve
 *  Fix name search in elab_lval.
 *
 * Revision 1.17  2003/01/30 16:23:08  steve
 *  Spelling fixes.
 *
 * Revision 1.16  2002/08/31 03:48:50  steve
 *  Fix reverse bit ordered bit select in continuous assignment.
 *
 * Revision 1.15  2002/08/12 01:35:00  steve
 *  conditional ident string using autoconfig.
 *
 * Revision 1.14  2002/06/25 01:33:22  steve
 *  Cache calculated driven value.
 *
 * Revision 1.13  2002/06/24 01:49:39  steve
 *  Make link_drive_constant cache its results in
 *  the Nexus, to improve cprop performance.
 *
 * Revision 1.12  2001/02/15 06:59:36  steve
 *  FreeBSD port has a maintainer now.
 *
 * Revision 1.11  2001/02/10 20:29:39  steve
 *  In the context of range declarations, use elab_and_eval instead
 *  of the less robust eval_const methods.
 *
 * Revision 1.10  2000/11/20 00:58:40  steve
 *  Add support for supply nets (PR#17)
 *
 * Revision 1.9  2000/09/20 02:53:15  steve
 *  Correctly measure comples l-values of assignments.
 *
 * Revision 1.8  2000/06/25 19:59:42  steve
 *  Redesign Links to include the Nexus class that
 *  carries properties of the connected set of links.
 *
 * Revision 1.7  2000/05/14 17:55:04  steve
 *  Support initialization of FF Q value.
 */
#endif
