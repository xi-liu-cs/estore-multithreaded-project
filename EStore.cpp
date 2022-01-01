#include <cassert>
#include <cstdio>
#include <bits/stdc++.h>

#include "EStore.h"

using namespace std;


Item::
Item() : valid(false)
{
}

Item::
~Item()
{
}


EStore::
EStore(bool enableFineMode)
    : fineMode(enableFineMode)
{
    // TODO: Your code here.
    smutex_init(&mutex);
    scond_init(&ready);
    if(fineModeEnabled())
    {
        for(int i = 0; i < INVENTORY_SIZE; i++)
        {
            smutex_init(&mutex_array[i]);
        }
    }
}

EStore::
~EStore()
{
    // TODO: Your code here.
    smutex_destroy(&mutex);
    scond_destroy(&ready);
    if(fineModeEnabled())
    {
        for(int i = 0; i < INVENTORY_SIZE; i++)
        {
            smutex_destroy(&mutex_array[i]);
        }
    }
}

double EStore::
cost_item(Item * item_p)
{
    double item_cost = (item_p->price)*(1 - item_p->discount);
    return item_cost*(1 - store_discount) + ship_cost;
}

void EStore::
handle_mode(int item_id)
{
    if(fineModeEnabled() == false)
    {
        smutex_lock(&mutex);
    }
    else
    {
        smutex_lock(&mutex_array[item_id]);
    }
}

void EStore::
handle_mode_end(int item_id)
{
    if(fineModeEnabled() == false)
    {
        smutex_unlock(&mutex);
    }
    else
    {
        smutex_unlock(&mutex_array[item_id]);
    }
}
/*
 * ------------------------------------------------------------------
 * buyItem --
 *
 *      Attempt to buy the item from the store.
 *
 *      An item can be bought if:
 *          - The store carries it.
 *          - The item is in stock.
 *          - The cost of the item plus the cost of shipping is no
 *            more than the budget.
 *
 *      If the store *does not* carry this item, simply return and
 *      do nothing. Do not attempt to buy the item.
 *
 *      If the store *does* carry the item, but it is not in stock
 *      or its cost is over budget, block until both conditions are
 *      met (at which point the item should be bought) or the store
 *      removes the item from sale (at which point this method
 *      returns).
 *
 *      The overall cost of a purchase for a single item is defined
 *      as the current cost of the item times 1 - the store
 *      discount, plus the flat overall store shipping fee.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
buyItem(int item_id, double budget)
{
    assert(!fineModeEnabled());

    // TODO: Your code here.
    smutex_lock(&mutex);

    Item * item_p = inventory + item_id;

    while(item_p->valid &&
    ((item_p->quantity == 0) || (cost_item(item_p) > budget)))
    {//not in stock or cost is over budget
        scond_wait(&ready, &mutex); //block until both conditions are met
    }

    if(item_p->valid)
    {
        item_p->quantity--;
    }

    smutex_unlock(&mutex); 
}

/*
 * ------------------------------------------------------------------
 * buyManyItem --
 *
 *      Attempt to buy all of the specified items at once. If the
 *      order cannot be bought, give up and return without buying
 *      anything. Otherwise buy the entire order at once.
 *
 *      The entire order can be bought if:
 *          - The store carries all items.
 *          - All items are in stock.
 *          - The cost of the the entire order (cost of items plus
 *            shipping for each item) is no more than the budget.
 *
 *      If multiple customers are attempting to buy at the same
 *      time and their orders are mutually exclusive (i.e., the
 *      two customers are not trying to buy any of the same items),
 *      then their orders must be processed at the same time.
 *
 *      For the purposes of this lab, it is OK for the store
 *      discount and shipping cost to change while an order is being
 *      processed.
 *
 *      The cost of a purchase of many items is the sum of the
 *      costs of purchasing each item individually. The purchase
 *      cost of an individual item is covered above in the
 *      description of buyItem.
 *
 *      Challenge: For bonus points, implement a version of this
 *      method that will wait until the order can be fulfilled
 *      instead of giving up. The implementation should be efficient
 *      in that it should not wake up threads unecessarily. For
 *      instance, if an item decreases in price, only threads that
 *      are waiting to buy an order that includes that item should be
 *      signaled (though all such threads should be signaled).
 *
 *      Challenge: For bonus points, ensure that the shipping cost
 *      and store discount does not change while processing an
 *      order.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
buyManyItems(vector<int>* item_ids, double budget)
{
    assert(fineModeEnabled());

    // TODO: Your code here.
    sort(item_ids->begin(), item_ids->end()); //enforce lock ordering to avoid deadlocks

    for(long unsigned int i = 0; i < item_ids->size(); i++)
    {//lock all the items in the entire order
        smutex_lock(&mutex_array[item_ids->at(i)]);
    }

    int entire_cost = 0;
    bool buyable = true;

    for(long unsigned int i = 0; i < item_ids->size(); i++)
    {
        Item * item_p = inventory + item_ids->at(i);
        entire_cost += cost_item(item_p);
        if((item_p->valid == false) || (item_p->quantity == 0) || (entire_cost > budget))
        {
            buyable = false;
            break; //the entire order cannot be bought
        }
    }


    if(buyable)
    {//buy everything
        for(long unsigned int i = 0; i < item_ids->size(); i++)
        {
            Item * item_p = inventory + item_ids->at(i);
            item_p->quantity--;
        }
    }

    for(long unsigned int i = 0; i < item_ids->size(); i++)
    {//unlock all the items in the entire order
        smutex_unlock(&mutex_array[item_ids->at(i)]);
    }
}

/*
 * ------------------------------------------------------------------
 * addItem --
 *
 *      Add the item to the store with the specified quantity,
 *      price, and discount. If the store already carries an item
 *      with the specified id, do nothing.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
addItem(int item_id, int quantity, double price, double discount)
{
    // TODO: Your code here.
    handle_mode(item_id);
    Item * item_p = inventory + item_id;
    if(item_p->valid == false)
    {//store does not carry an item with the specified id
        item_p->quantity = quantity;
        item_p->price = price;
        item_p->discount = discount;
        item_p->valid = true;
    }
    handle_mode_end(item_id);
}

/*
 * ------------------------------------------------------------------
 * removeItem --
 *
 *      Remove the item from the store. The store no longer carries
 *      this item. If the store is not carrying this item, do
 *      nothing.
 *
 *      Wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
removeItem(int item_id)
{
    // TODO: Your code here.
    handle_mode(item_id);
    Item * item_p = inventory + item_id;
    if(item_p->valid == true)
    {//the store is carrying this item
        item_p->valid = false;
        scond_broadcast(&ready, &mutex);
    }
    handle_mode_end(item_id);
}

/*
 * ------------------------------------------------------------------
 * addStock --
 *
 *      Increase the stock of the specified item by count. If the
 *      store does not carry the item, do nothing. Wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
addStock(int item_id, int count)
{
    // TODO: Your code here
    handle_mode(item_id);
    Item * item_p = inventory + item_id;
    if(item_p->valid == true)
    {//the store does carry the item   
        item_p->quantity += count;
        scond_broadcast(&ready, &mutex);
    }
    handle_mode_end(item_id);
}

/*
 * ------------------------------------------------------------------
 * priceItem --
 *
 *      Change the price on the item. If the store does not carry
 *      the item, do nothing.
 *
 *      If the item price decreased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
priceItem(int item_id, double price)
{
    // TODO: Your code here.
    handle_mode(item_id);
    Item * item_p = inventory + item_id;
    if(item_p->valid == true)
    {//the store does carry the item
        if(price < item_p->price)
        {//the price decreased
            scond_broadcast(&ready, &mutex);
        }
        item_p->price = price;
    }
    handle_mode_end(item_id);
}

/*
 * ------------------------------------------------------------------
 * discountItem --
 *
 *      Change the discount on the item. If the store does not carry
 *      the item, do nothing.
 *
 *      If the item discount increased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
discountItem(int item_id, double discount)
{
    // TODO: Your code here.
    handle_mode(item_id);
    Item * item_p = inventory + item_id;
    if(item_p->valid == true)
    {//the store does carry the item
        if(discount > item_p->discount)
        {//the discount increased
            scond_broadcast(&ready, &mutex);
        }
        item_p->discount = discount;
    }
    handle_mode_end(item_id);
}

/*
 * ------------------------------------------------------------------
 * setShippingCost --
 *
 *      Set the per-item shipping cost. If the shipping cost
 *      decreased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
setShippingCost(double cost)
{
    // TODO: Your code here.
    smutex_lock(&mutex);
    if(cost < ship_cost)
    {//shipping cost decreased
        scond_broadcast(&ready, &mutex);
    }
    ship_cost = cost;
    smutex_unlock(&mutex);
}

/*
 * ------------------------------------------------------------------
 * setStoreDiscount --
 *
 *      Set the store discount. If the discount increased, wake any
 *      waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
setStoreDiscount(double discount)
{
    // TODO: Your code here.
    smutex_lock(&mutex);
    if(discount > store_discount)
    {//the discount increased
        scond_broadcast(&ready, &mutex);
    }
    store_discount = discount;
    smutex_unlock(&mutex);
}


