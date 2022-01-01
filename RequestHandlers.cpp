#include <iostream>

#include "EStore.h"
#include "TaskQueue.h"

using namespace std;
/*
 * ------------------------------------------------------------------
 * add_item_handler --
 *
 *      Handle an AddItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
add_item_handler(void *args)
{
    // TODO: Your code here.
    AddItemReq * add_item_req = (AddItemReq *)args;

    printf("Handling AddItemReq: item_id - %d, "
    "quantity - %d, " 
    "price - %0.2f, "
    "discount - %0.2f\n",
    add_item_req->item_id,
    add_item_req->quantity,
    add_item_req->price,
    add_item_req->discount);

    add_item_req->store->addItem(add_item_req->item_id,
    add_item_req->quantity,
    add_item_req->price,
    add_item_req->discount);

    delete add_item_req;
}

/*
 * ------------------------------------------------------------------
 * remove_item_handler --
 *
 *      Handle a RemoveItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
remove_item_handler(void *args)
{
    // TODO: Your code here.
    RemoveItemReq * rm_item_req = (RemoveItemReq *)args;

    printf("Handling RemoveItemReq: item_id - %d\n",
    rm_item_req->item_id);

    rm_item_req->store->removeItem(rm_item_req->item_id);

    delete rm_item_req;
}

/*
 * ------------------------------------------------------------------
 * add_stock_handler --
 *
 *      Handle an AddStockReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
add_stock_handler(void *args)
{
    // TODO: Your code here.
    AddStockReq * add_stock_req = (AddStockReq *)args;

    printf("Handling AddStockReq: item_id - %d, "
    "additional_stock - %d\n",
    add_stock_req->item_id,
    add_stock_req->additional_stock);

    add_stock_req->store->addStock(add_stock_req->item_id,
    add_stock_req->additional_stock);

    delete add_stock_req;
}

/*
 * ------------------------------------------------------------------
 * change_item_price_handler --
 *
 *      Handle a ChangeItemPriceReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
change_item_price_handler(void *args)
{
    // TODO: Your code here.
    ChangeItemPriceReq * change_item_price_req = (ChangeItemPriceReq *)args;

    printf("Handling ChangeItemPriceReq: item_id - %d, "
    "new_price - %0.2f\n",
    change_item_price_req->item_id,
    change_item_price_req->new_price);
     
    change_item_price_req->store->priceItem(change_item_price_req->item_id,
    change_item_price_req->new_price);

    delete change_item_price_req;
}

/*
 * ------------------------------------------------------------------
 * change_item_discount_handler --
 *
 *      Handle a ChangeItemDiscountReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
change_item_discount_handler(void *args)
{
    // TODO: Your code here.
    ChangeItemDiscountReq * change_item_discount_req = (ChangeItemDiscountReq *)args;

    printf("Handling ChangeItemDiscountReq: item_id - %d, "
    "new_discount - %0.2f\n",
    change_item_discount_req->item_id,
    change_item_discount_req->new_discount);

    change_item_discount_req->store->discountItem(change_item_discount_req->item_id,
    change_item_discount_req->new_discount);

    delete change_item_discount_req;
}

/*
 * ------------------------------------------------------------------
 * set_shipping_cost_handler --
 *
 *      Handle a SetShippingCostReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
set_shipping_cost_handler(void *args)
{
    // TODO: Your code here.
    SetShippingCostReq * set_shipping_cost_req = (SetShippingCostReq *)args;

    printf("Handling ShippingCostReq: new shipping cost - $%0.2f\n",
    set_shipping_cost_req->new_cost);

    set_shipping_cost_req->store->setShippingCost(set_shipping_cost_req->new_cost);

    delete set_shipping_cost_req;
}

/*
 * ------------------------------------------------------------------
 * set_store_discount_handler --
 *
 *      Handle a SetStoreDiscountReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
set_store_discount_handler(void *args)
{
    // TODO: Your code here.
    SetStoreDiscountReq * set_store_discount_req = (SetStoreDiscountReq *)args;

    printf("Handling storeDiscountReq: new discount - %0.2f\n",
    set_store_discount_req->new_discount);

    set_store_discount_req->store->setStoreDiscount(set_store_discount_req->new_discount);

    delete set_store_discount_req;
}

/*
 * ------------------------------------------------------------------
 * buy_item_handler --
 *
 *      Handle a BuyItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
buy_item_handler(void *args)
{
    // TODO: Your code here.
    BuyItemReq * buy_item_req = (BuyItemReq *)args;

    printf("Handling BuyItemReq: item_id - %d, "
    "budget - $%0.2f\n",
    buy_item_req->item_id,
    buy_item_req->budget);

    buy_item_req->store->buyItem(buy_item_req->item_id, 
    buy_item_req->budget);

    delete buy_item_req;
}

/*
 * ------------------------------------------------------------------
 * buy_many_items_handler --
 *
 *      Handle a BuyManyItemsReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
buy_many_items_handler(void *args)
{
    // TODO: Your code here.
    BuyManyItemsReq * buy_many_items_req = (BuyManyItemsReq *)args;

    printf("Handling BuyManyItemsReq: item_id - ");
    for(long unsigned int i = 0; i < buy_many_items_req->item_ids.size(); i++)
    {
        printf("%d ", buy_many_items_req->item_ids.at(i));
    }
    printf("\n");

    buy_many_items_req->store->buyManyItems(&(buy_many_items_req->item_ids),
    buy_many_items_req->budget);

    delete buy_many_items_req;
}

/*
 * ------------------------------------------------------------------
 * stop_handler --
 *
 *      The thread should exit.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
stop_handler(void* args)
{
    // TODO: Your code here.
    printf("Handling StopHandlerReq: Quitting.\n");
    sthread_exit();
}

