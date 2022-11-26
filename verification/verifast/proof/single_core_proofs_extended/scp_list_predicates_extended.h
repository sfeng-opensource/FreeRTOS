#ifndef SCP_LIST_PREDICATES_EXTENDED_H
#define SCP_LIST_PREDICATES_EXTENDED_H

#include "single_core_proofs/scp_list_predicates.h"

/* =============================================================================
 * The lemmas below assist in opening and closing DLS predicates in a way that
 * allows accesses to `pxItem->pxNext`.
*/

/*@
lemma void DLS_end_next_open(struct xLIST* pxList, struct xLIST_ITEM* pxItem)
requires
    DLS(?gEnd, ?gEndPrev, gEnd, gEndPrev, ?gCells, ?gVals, pxList) &*&
    mem(pxItem, gCells) == true &*&
    gEnd == head(gCells) &*&
    length(gCells) == length(gVals) &*&
    length(gCells) > 1 
    &*&
    pxItem == gEnd;
ensures 
    xLIST_ITEM(gEnd, head(gVals), ?gItem_next, gEndPrev, pxList) &*&
    DLS(gItem_next, gEnd, gEnd, gEndPrev, drop(1, gCells), drop(1, gVals), pxList ) &*&
    mem(gItem_next, gCells) == true;
{
    open DLS(gEnd, gEndPrev, gEnd, gEndPrev, gCells, gVals, pxList);
    // open DLS and xLIST_ITEM predicates to justify
    // accessing `pxItem->pxNext`
    assert( xLIST_ITEM(gEnd, ?gItemVal, ?gItem_next, gEndPrev, pxList) );
    open xLIST_ITEM(gEnd, gItemVal, gItem_next, gEndPrev, pxList);
    assert( DLS(gItem_next, gEnd, gEnd, gEndPrev, 
                drop(1, gCells), drop(1, gVals), pxList ) );
    open DLS(gItem_next, gEnd, gEnd, gEndPrev, 
             drop(1, gCells), drop(1, gVals), pxList );
    
    // open DLS and xLIST_ITEM predicates to prove
    // `mem( pxItem->pxNext, gCells) == true )`
    // which requires accessing `pxItem->pxNext`
    assert( xLIST_ITEM(gItem_next, ?gItem_nextVal, ?gItem_nextNext, gEnd, pxList) );
    open xLIST_ITEM(gItem_next, gItem_nextVal, gItem_nextNext, gEnd, pxList);
    assert( mem(pxItem->pxNext, gCells) == true );
    close xLIST_ITEM(gItem_next, gItem_nextVal, gItem_nextNext, gEnd, pxList);

    // closing what we opened above
    close DLS(gItem_next, gEnd, gEnd, gEndPrev, 
              drop(1, gCells), drop(1, gVals), pxList );
    close xLIST_ITEM(gEnd, gItemVal, gItem_next, gEndPrev, pxList);
}


lemma void DLS_end_next_close(struct xLIST* pxList, struct xLIST_ITEM* pxItem)
requires 
    xLIST_ITEM(?gEnd, ?gItemVal, ?gItem_next, ?gEndPrev, pxList) &*&
    DLS(gItem_next, gEnd, gEnd, gEndPrev, ?gCells, ?gVals, pxList) &*&
//    mem(gItem_next, gCells) == true &*&
    length(gCells) == length(gVals) &*&
    length(gCells) > 0 &*&
    pxItem == gEnd;
ensures
    DLS(gEnd, gEndPrev, gEnd, gEndPrev, 
        cons(gEnd, gCells), cons(gItemVal, gVals), pxList);
{
    open DLS(gItem_next, gEnd, gEnd, gEndPrev, gCells, gVals, pxList);
    close DLS(gItem_next, gEnd, gEnd, gEndPrev, gCells, gVals, pxList);
    dls_star_item(gItem_next, gEndPrev, gEnd);
    dls_distinct(gItem_next, gEnd, gEnd, gEndPrev, gCells);
    dls_last_mem(gItem_next, gEnd, gEnd, gEndPrev, gCells);
    close DLS(gEnd, gEndPrev, gEnd, gEndPrev, 
        	  cons(gEnd, gCells), cons(gItemVal, gVals), pxList);
}


lemma void DLS_nonEndItem_next_open(struct xLIST* pxList, struct xLIST_ITEM* pxItem)
requires
    DLS(?gEnd, ?gEndPrev, gEnd, gEndPrev, ?gCells, ?gVals, pxList) &*&
    mem(pxItem, gCells) == true &*&
    gEnd == head(gCells) &*&
    length(gCells) == length(gVals) &*&
    length(gCells) > 1 
    &*&
    pxItem != gEnd;
ensures 
    // DLS prefix
    DLS(gEnd, gEndPrev, pxItem, ?pxItem_prev, 
        take(index_of(pxItem, gCells), gCells), 
        take(index_of(pxItem, gCells), gVals),
        pxList)
    &*&
    // item of interest
    xLIST_ITEM(pxItem, ?gItemVal, ?pxItem_next, pxItem_prev, pxList) &*&
    gItemVal == nth(index_of(pxItem, gCells), gVals)
    &*&
    // DLS suffix
    (pxItem != gEndPrev
        ? DLS(pxItem_next, pxItem, gEnd, gEndPrev, 
                drop(1, drop(index_of(pxItem, gCells), gCells)), 
                drop(1, drop(index_of(pxItem, gCells), gVals)),
                pxList)
        : (pxItem_next == gEnd &*&
            index_of(pxItem, gCells) == length(gCells) - 1
          )
    )
    &*&
    mem(pxItem_next, gCells) == true;
{
    int pxItemIndex_0 = index_of(pxItem, gCells);


    // open DLS and xLIST_ITEM predicates to justify
    // accessing `pxItem->pxNext`
    split(gEnd, gEndPrev, gEnd, gEndPrev, 
            gCells, gVals, pxItem, pxItemIndex_0);
    // DLS prefix
    assert( DLS(gEnd, gEndPrev, pxItem, ?pxItem_prev, 
                take(pxItemIndex_0, gCells), take(pxItemIndex_0, gVals),
                pxList) );
    // DLS suffix
    assert( DLS(pxItem, pxItem_prev, gEnd, gEndPrev, 
                drop(pxItemIndex_0, gCells), drop(pxItemIndex_0, gVals),
                pxList) );
    open DLS(pxItem, pxItem_prev, gEnd, gEndPrev, 
                drop(pxItemIndex_0, gCells), drop(pxItemIndex_0, gVals),
                pxList);
    assert( xLIST_ITEM(pxItem, ?gItemVal, ?pxItem_next, pxItem_prev, pxList) );
    assert( gItemVal == head(drop(pxItemIndex_0, gVals)) );
    head_drop_n_equals_nths(gVals, pxItemIndex_0);
    assert( gItemVal == nth(index_of(pxItem, gCells), gVals) );
    
    
    // open DLS and xLIST_ITEM predicates to prove
    // `mem( pxItem->pxNext, gCells) == true )`
    // which requires accessing `pxItem->pxNext`
    if(pxItem == gEndPrev) {
        assert( drop(pxItemIndex_0, gCells) == cons(pxItem, nil) );
        drop_index_equals_singleton_implies_last_element(gCells, pxItem);
        assert( pxItemIndex_0 == length(gCells) - 1 );

        // `pxItem` is last element in DLS suffix
        // -> `pxItem_next` is head fo DLS prefix
        // open DLS prefix
        open xLIST_ITEM(pxItem, gItemVal, pxItem_next, pxItem_prev, pxList);
        assert( gCells == cons(_, _) );
        assert( mem(pxItem->pxNext, gCells) == true );
        
        // close item of interest
        close xLIST_ITEM(pxItem, gItemVal, pxItem_next, pxItem_prev, pxList);
    } else {
        // `pxItem` is not end of DLS suffix
        // -> `pxItem_next` is also in DLS suffix
        // open DLS suffix one step further
        
        // rest of DLS suffix
        assert( DLS(pxItem_next, pxItem, gEnd, gEndPrev, 
                    drop(1, drop(pxItemIndex_0, gCells)), 
                    drop(1, drop(pxItemIndex_0, gVals)),
                    pxList) );
        open DLS(pxItem_next, pxItem, gEnd, gEndPrev, 
                 drop(1, drop(pxItemIndex_0, gCells)), 
                 drop(1, drop(pxItemIndex_0, gVals)),
                 pxList);
        assert( xLIST_ITEM(pxItem_next, ?gItem_nextVal, ?pxItem_next_next, pxItem, pxList) );
        open xLIST_ITEM(pxItem, gItemVal, pxItem_next, pxItem_prev, pxList); 
        mem_suffix_implies_mem(pxItem_next, gCells, pxItemIndex_0);
        assert( mem(pxItem->pxNext, gCells) == true );

        // close rest of DLS suffix
        close xLIST_ITEM(pxItem_next, gItem_nextVal, pxItem_next_next, pxItem, pxList);
        close DLS(pxItem_next, pxItem, gEnd, gEndPrev, 
                    drop(1, drop(pxItemIndex_0, gCells)), 
                    drop(1, drop(pxItemIndex_0, gVals)),
                    pxList);

        // close item of interest
        close xLIST_ITEM(pxItem, gItemVal, pxItem_next, pxItem_prev, pxList); 
    }
}


lemma void DLS_nonEndItem_next_close(struct xLIST* pxList, struct xLIST_ITEM* pxItem, 
                                     list<struct xLIST_ITEM*> gCells,
                                     list<TickType_t> gVals)
requires
    length(gCells) == length(gVals) &*&
    length(gCells) > 1 
    &*&
    // DLS prefix
    DLS(?gEnd, ?gEndPrev, pxItem, ?gItem_prev, ?gCellsPrefix, ?gValsPrefix,
        pxList)
    &*&
    mem(pxItem, gCells) == true &*&
    gCellsPrefix == take(index_of(pxItem, gCells), gCells) &*&
    gValsPrefix == take(index_of(pxItem, gCells), gVals) 
    &*&
    // item of interest
    pxItem != gEnd &*&
    xLIST_ITEM(pxItem, ?gItemVal, ?gItem_next, gItem_prev, pxList) &*&
    mem(gItemVal, gVals) == true &*&
    gItemVal == nth(index_of(pxItem, gCells), gVals)
    &*&
    // DLS suffix
    (pxItem != gEndPrev
        ? DLS(gItem_next, pxItem, gEnd, gEndPrev, 
                drop(1, drop(index_of(pxItem, gCells), gCells)), 
                drop(1, drop(index_of(pxItem, gCells), gVals)),
                pxList)
        : (gItem_next == gEnd &*&
            index_of(pxItem, gCells) == length(gCells) - 1
          )
    )
    &*&
    mem(gItem_next, gCells) == true;
ensures
    DLS(gEnd, gEndPrev, gEnd, gEndPrev, gCells, gVals, pxList);
{
    int gItemIndex = index_of(pxItem, gCells);
    head_drop_n_equals_nths(gCells, gItemIndex);
    head_drop_n_equals_nths(gVals, gItemIndex);
    
    if( pxItem != gEndPrev ) {
        assert( drop(gItemIndex, gVals) == cons(_, _) );
        assert( xLIST_ITEM(pxItem, ?gV, _, gItem_prev, pxList) );
        nth_index(gCells, pxItem);
        close DLS(pxItem, gItem_prev, gEnd, gEndPrev, 
                  drop(gItemIndex, gCells), drop(gItemIndex, gVals),
                  pxList);
        join(gEnd, gEndPrev, pxItem, gItem_prev, gCellsPrefix, gValsPrefix,
             pxItem, gItem_prev, gEnd, gEndPrev, drop(gItemIndex, gCells), drop(gItemIndex, gVals));
    } else {
        assert( xLIST_ITEM(pxItem, ?gV, ?gNext, gItem_prev, pxList) );
        assert( xLIST_ITEM(pxItem, gV, gEnd, gItem_prev, pxList) );
        close DLS(pxItem, gItem_prev, gEnd, gEndPrev, cons(pxItem, nil), cons(gItemVal, nil), pxList);
        join(gEnd, gEndPrev, pxItem, gItem_prev, gCellsPrefix, gValsPrefix,
             pxItem, gItem_prev, gEnd, gEndPrev, cons(pxItem, nil), cons(gItemVal, nil));
        assert( DLS(gEnd, gEndPrev, gEnd, gEndPrev, ?gCellsRes, ?gValsRes, pxList));

        assert( gCellsPrefix == take(index_of(pxItem, gCells), gCells) );
        assert( gValsPrefix == take(index_of(pxItem, gCells), gVals) );
        assert( gCellsRes == append(gCellsPrefix, cons(pxItem, nil)) );
        assert( gValsRes == append(gValsPrefix, cons(gItemVal, nil)) );
        
        
        drop_cons(gCells, index_of(pxItem, gCells));
        drop_cons(gVals, index_of(pxItem, gCells));
        nth_index(gCells, pxItem);

        assert( gCellsRes == gCells );
        assert( gValsRes == gVals );
    }
}


lemma void DLS_next_open(struct xLIST* pxList, struct xLIST_ITEM* pxItem)
requires
    DLS(?gEnd, ?gEndPrev, gEnd, gEndPrev, ?gCells, ?gVals, pxList) &*&
    mem(pxItem, gCells) == true &*&
    gEnd == head(gCells) &*&
    length(gCells) == length(gVals) &*&
    length(gCells) > 1;
ensures
    pxItem == gEnd
        ? (
            xLIST_ITEM(gEnd, head(gVals), ?gItem_next, gEndPrev, pxList) &*&
            DLS(gItem_next, gEnd, gEnd, gEndPrev, drop(1, gCells), drop(1, gVals), pxList ) &*&
            mem(gItem_next, gCells) == true
        )
        : (
            // DLS prefix
            DLS(gEnd, gEndPrev, pxItem, ?gItem_prev, 
                take(index_of(pxItem, gCells), gCells), 
                take(index_of(pxItem, gCells), gVals),
                pxList)
            &*&
            // item of interest
            xLIST_ITEM(pxItem, ?gItemVal, ?pxItem_next, gItem_prev, pxList) &*&
            gItemVal == nth(index_of(pxItem, gCells), gVals)
            &*&
            // DLS suffix
            (pxItem != gEndPrev
                ? DLS(pxItem_next, pxItem, gEnd, gEndPrev, 
                        drop(1, drop(index_of(pxItem, gCells), gCells)), 
                        drop(1, drop(index_of(pxItem, gCells), gVals)),
                        pxList)
                : (pxItem_next == gEnd &*&
                    index_of(pxItem, gCells) == length(gCells) - 1
                )
            )
            &*&
            mem(pxItem_next, gCells) == true
        );
{
    if( pxItem == gEnd ) {
        DLS_end_next_open(pxList, pxItem);
    } else {
        DLS_nonEndItem_next_open(pxList, pxItem);
    }
}


lemma void DLS_next_close(struct xLIST* pxList, struct xLIST_ITEM* pxItem,
                         list<struct xLIST_ITEM*> gCells,
                         list<TickType_t> gVals,
                         struct xLIST_ITEM* gEnd,
                         struct xLIST_ITEM* gEndPrev)
requires
    head(gCells) == gEnd &*&
    length(gCells) > 1 &*&
    length(gCells) == length(gVals) &*&
    pxItem == gEnd
        ? (
            xLIST_ITEM(gEnd, ?gItemVal, ?gItem_next, gEndPrev, pxList) &*&
            DLS(gItem_next, gEnd, gEnd, gEndPrev, drop(1, gCells), drop(1, gVals), pxList) &*&
            length(gCells) == length(gVals) &*&
            length(gCells) > 0 &*&
            head(gVals) == gItemVal
        )
        : (
            length(gCells) == length(gVals) &*&
            length(gCells) > 1 
            &*&
            // DLS prefix
            DLS(gEnd, gEndPrev, pxItem, ?gItem_prev, ?gCellsPrefix, ?gValsPrefix,
                pxList)
            &*&
            mem(pxItem, gCells) == true &*&
            gCellsPrefix == take(index_of(pxItem, gCells), gCells) &*&
            gValsPrefix == take(index_of(pxItem, gCells), gVals) 
            &*&
            // item of interest
            pxItem != gEnd &*&
            xLIST_ITEM(pxItem, ?gItemVal, ?gItem_next, gItem_prev, pxList) &*&
            mem(gItemVal, gVals) == true &*&
            gItemVal == nth(index_of(pxItem, gCells), gVals)
            &*&
            // DLS suffix
            (pxItem != gEndPrev
                ? DLS(gItem_next, pxItem, gEnd, gEndPrev, 
                        drop(1, drop(index_of(pxItem, gCells), gCells)), 
                        drop(1, drop(index_of(pxItem, gCells), gVals)),
                        pxList)
                : (gItem_next == gEnd &*&
                    index_of(pxItem, gCells) == length(gCells) - 1
                )
            )
            &*&
            mem(gItem_next, gCells) == true
        );
ensures
    DLS(gEnd, gEndPrev, gEnd, gEndPrev, gCells, gVals, pxList);
{
    if( pxItem == gEnd ) {
        DLS_end_next_close(pxList, pxItem);

        // why is this necessary?
        assert( gCells == cons( _, _) );    
        assert( gVals == cons(_, _) );
    } else {
        DLS_nonEndItem_next_close(pxList, pxItem, gCells, gVals);
    }
}
@*/

/* By verifying the following function, we can validate that the above lemmas
 * apply to the use cases they are meant for.
 */
void lemma_validation__DLS_item_next(struct xLIST_ITEM* pxTaskItem)
/*@ requires
        DLS(?gEnd, ?gEndPrev, gEnd, gEndPrev, ?gCells, ?gVals, ?gList) &*&
        mem(pxTaskItem, gCells) == true &*&
        gEnd == head(gCells) &*&
        length(gCells) == length(gVals) &*&
        length(gCells) > 1;
@*/
/*@ ensures
        DLS(gEnd, gEndPrev, gEnd, gEndPrev, gCells, gVals, gList) &*&
        mem(pxTaskItem, gCells) == true;
@*/
{
    //@ struct xLIST_ITEM* gTaskItem_0 = pxTaskItem;

    /* @
    if( gTaskItem_0 == gEnd ) {
        DLS_end_next_open(gList, gTaskItem_0);
    } else {
        DLS_nonEndItem_next_open(gList, gTaskItem_0);
    }
    @*/

    //@ DLS_next_open(gList, gTaskItem_0);

    pxTaskItem = pxTaskItem->pxNext;
    //@ struct xLIST_ITEM* pxItem_1 = pxTaskItem;

    
    //@ close xLIST_ITEM(gTaskItem_0, ?gTaskItemVal, _, _, gList);

    //@ DLS_next_close(gList, gTaskItem_0, gCells, gVals, gEnd, gEndPrev);

    /* @
    if( gTaskItem_0 == gEnd ) {
        DLS_end_next_close(gList, gTaskItem_0);
        assert( DLS(gEnd, gEndPrev, gEnd, gEndPrev, ?gCells2, ?gVals2, gList) );

        // why is this necessary?
        assert( gCells == cons( _, _) );    
        assert( gVals == cons(_, _) );
    } else {
        DLS_nonEndItem_next_close(gList, gTaskItem_0, gCells, gVals);
    }
    @*/

    //@ assert( mem(pxItem_1, gCells) == true );
}



/* ----------------------------------------
 * The folling lemmas aim to simpilfy the lemmas above and reduce
 * the number of case distinctions that are introduced by applying them.
 */


/*@
// Splitting a full DLS of the form
// DLS(end, endPrev, end, endPrev, cells, vals, list)
// at item `I` should result in a prefix, the item of interest and a suffix.
// Both prefix and suffix can be empty, which the standard DLS predicate does
// not allow
predicate DLS_prefix(
        // prefix args
        list<struct xLIST_ITEM * > prefCells,
        list<TickType_t > prefVals,
        struct xLIST_ITEM* item,
        struct xLIST_ITEM* itemPrev,
        // unsplit DLS args
        struct xLIST_ITEM *end,
        struct xLIST_ITEM *endPrev,
        struct xLIST *pxContainer) =
	switch(prefCells) {
        case nil: return true;
        case cons(headItem, tailCells): return 
            headItem == end &*&
            length(prefCells) == length(prefVals) &*&
            DLS(end, endPrev, item, itemPrev, prefCells, prefVals, pxContainer);
    };

predicate DLS_suffix(
        // suffix args
        list<struct xLIST_ITEM * > sufCells,
        list<TickType_t > sufVals,
        struct xLIST_ITEM* item,
        struct xLIST_ITEM* itemNext,
        // unsplit DLS args
        struct xLIST_ITEM *end,
        struct xLIST_ITEM *endPrev,
        struct xLIST *pxContainer) =
	switch(sufCells) {
        case nil: return true;
        case cons(headItem, tailCells): return 
            length(sufCells) == length(sufVals) &*&
            DLS(itemNext, item, end, endPrev, sufCells, sufVals, pxContainer);
    };


lemma void DLS_open_2(struct xLIST_ITEM* pxItem)
requires 
    DLS(?gEnd, ?gEndPrev, gEnd, gEndPrev, ?gCells, ?gVals, ?gList) &*&
    mem(pxItem, gCells) == true &*&
    gEnd == head(gCells) &*&
    length(gCells) == length(gVals) &*&
    length(gCells) > 1;
ensures 
    DLS_prefix(?gPrefCells, ?gPrefVals, pxItem, ?gItemPrev,
               gEnd, gEndPrev, gList)
    &*&
    xLIST_ITEM(pxItem, ?gItemVal, ?gItemNext, gItemPrev, gList)
    &*&
    DLS_suffix(?gSufCells, ?gSufVals, pxItem, gItemNext, gEnd, gEndPrev, gList) &*&
    true
    // gCells == gPrefCells + item + gSufCells
    // gVals == gPrefVals + item + gSufVals
    &*&
    // next in cells
    mem(gItemNext, gCells) == true &*&
    // prev in cells
    mem(gItemPrev, gCells) == true 
    ;
{
    if(pxItem == gEnd) {
        // pxItem is first/ left-most item in the list
        // -> empty prefix

        open DLS(gEnd, gEndPrev, gEnd, gEndPrev, gCells, gVals, gList);
        assert( xLIST_ITEM(pxItem, ?gItemVal, ?gItemNext, ?gItemPrev, gList) );
        assert( DLS(gItemNext, pxItem, gEnd, gEndPrev, ?gSufCells, ?gSufVals, 
                    gList) );
        close DLS_prefix(nil, nil, pxItem, gItemPrev, 
                         gEnd, gEndPrev, gList);

        // Prove: `mem(gItemNext, gCells) == true`
            open DLS(gItemNext, pxItem, gEnd, gEndPrev, gSufCells, gSufVals, gList);
            assert( mem(gItemNext, gCells) == true );
            close DLS(gItemNext, pxItem, gEnd, gEndPrev, gSufCells, gSufVals, gList);

        // Prove: `mem(gItemPrev, gCells) == true `
            assert( gItemPrev == gEndPrev );
            dls_last_mem(gItemNext, pxItem, gEnd, gEndPrev, gSufCells);
            assert( mem(gItemPrev, gCells) == true );

        close DLS_suffix(gSufCells, gSufVals, pxItem, gItemNext, gEnd, gEndPrev, 
                         gList);
    } else {
        // pxItem is not the first/ left-most item in the list
        // -> non-empty prefix
        // (potentially empty suffix)

        int gItemIndex = index_of(pxItem, gCells);
        split(gEnd, gEndPrev, gEnd, gEndPrev, gCells, gVals, pxItem, gItemIndex);

        assert( DLS(gEnd, gEndPrev, pxItem, ?gItemPrev, ?gPrefCells, ?gPrefVals, 
                    gList) );
        // -> Will be wrapped inside the prefix constructed at the end of this
        //    lemma.

        assert( DLS(pxItem, gItemPrev, gEnd, gEndPrev, ?gPartCells, ?gPartVals, 
                    gList) );
        // -> The tail of this DLS will make up the suffix constructed at the
        //    end of this lemma.

        // Prove: `head(gPrefCells) == gEnd`
        // Necessary to construct prefix later.
        // Implies `mem(gItemPrev, gCells) == true`.
            open DLS(gEnd, gEndPrev, pxItem, gItemPrev, gPrefCells, gPrefVals, 
                     gList);
            assert( head(gPrefCells) == gEnd );
            close DLS(gEnd, gEndPrev, pxItem, gItemPrev, gPrefCells, gPrefVals, 
                     gList);
            assert( mem(gItemPrev, gCells) == true );

        open DLS(pxItem, gItemPrev, gEnd, gEndPrev, gPartCells, gPartVals, 
                gList);
        assert( xLIST_ITEM(pxItem, ?gItemVal, ?gItemNext, gItemPrev, gList) );

        if( pxItem == gEndPrev ) {
            // pxItem is the last/ right-most item in the list.
            // -> empty suffix
            assert( gItemNext == gEnd );

            // prove: `mem(gItemNext, gCells) == true`
            dls_first_mem(gEnd, gEndPrev, pxItem, gItemPrev, gPrefCells);
            assert( mem(gItemNext, gPrefCells) == true );
            assert( gPrefCells == take(gItemIndex, gCells) );
            mem_prefix_implies_mem(gCells, gItemNext, gItemIndex);
            assert( mem(gItemNext, gCells) == true );


            // prove: mem(gItemNext, gCells) == true
                open xLIST_ITEM(pxItem, gItemVal, gItemNext, gItemPrev, gList);
                assert( gItemNext == gEnd );
                assert( mem(gItemNext, gCells) == true );
                close xLIST_ITEM(pxItem, gItemVal, gItemNext, gItemPrev, gList);

            close DLS_prefix(gPrefCells, gPrefVals, pxItem, gItemPrev,
                             gEnd, gEndPrev, gList);
            close DLS_suffix(nil, nil, pxItem, gItemNext, gEnd, gEndPrev, gList);
        } else {
            // pxItem is not the last/ right-most item in the list.
            // -> non-empty suffix

            assert( DLS(gItemNext, pxItem, gEnd, gEndPrev, ?gSufCells, ?gSufVals, 
                        gList) );

            // Prove: `mem(gItemNext, gCells) == true`
                open DLS(gItemNext, pxItem, gEnd, gEndPrev, gSufCells, gSufVals, 
                         gList);
                assert( mem(gItemNext, gSufCells) == true );
                assert( drop(gItemIndex, gCells) == cons(pxItem, gSufCells) );
                assert( drop(1, drop(gItemIndex, gCells)) == gSufCells );
                drop_n_plus_m(gCells, 1, gItemIndex);
                assert( drop(gItemIndex+1, gCells) == gSufCells );
                mem_suffix_implies_mem(gCells, gItemNext, gItemIndex+1);
                assert( mem(gItemNext, gCells) == true );
                close DLS(gItemNext, pxItem, gEnd, gEndPrev, gSufCells, gSufVals, 
                          gList);


     //   assert( xLIST_ITEM(pxItem, ?gItemVal, gItemNext, gItemPrev, gList) );

 //    close DLS_prefix(gPrefCells, gPrefVals, pxItem, gItemPrev,
  //                           gEnd, gEndPrev, gList);
   //         close DLS_suffix(nil, nil, pxItem, gItemNext, gEnd, gEndPrev, gList);
        }
    }
}
@*/

void lemma_validation__DLS_item_next_2(struct xLIST_ITEM* pxTaskItem)
/*@ requires
        DLS(?gEnd, ?gEndPrev, gEnd, gEndPrev, ?gCells, ?gVals, ?gList) &*&
        mem(pxTaskItem, gCells) == true &*&
        gEnd == head(gCells) &*&
        length(gCells) == length(gVals) &*&
        length(gCells) > 1;
@*/
/*@ ensures
        DLS(gEnd, gEndPrev, gEnd, gEndPrev, gCells, gVals, gList) &*&
        mem(pxTaskItem, gCells) == true;
@*/
{
    //@ struct xLIST_ITEM* gTaskItem_0 = pxTaskItem;

   

    pxTaskItem = pxTaskItem->pxNext;
    //@ struct xLIST_ITEM* pxItem_1 = pxTaskItem;


    //@ assert( mem(pxItem_1, gCells) == true );
}





#endif /* SCP_LIST_PREDICATES_EXTENDED_H */