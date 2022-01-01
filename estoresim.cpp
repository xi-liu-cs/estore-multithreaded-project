#include <cstring>
#include <cstdlib>

#include "EStore.h"
#include "TaskQueue.h"
#include "RequestGenerator.h"

class Simulation
{
    public:
    TaskQueue supplierTasks;
    TaskQueue customerTasks;
    EStore store;

    int maxTasks;
    int numSuppliers;
    int numCustomers;

    explicit Simulation(bool useFineMode) : store(useFineMode) { }
};

/*
 * ------------------------------------------------------------------
 * supplierGenerator --
 *
 *      The supplier generator thread. The argument is a pointer to
 *      the shared Simulation object.
 *
 *      Enqueue arg->maxTasks requests to the supplier queue, then
 *      stop all supplier threads by enqueuing arg->numSuppliers
 *      stop requests.
 *
 *      Use a SupplierRequestGenerator to generate and enqueue
 *      requests.
 *
 *      This thread should exit when done.
 *
 * Results:
 *      Does not return. Exit instead.
 *
 * ------------------------------------------------------------------
 */
static void*
supplierGenerator(void* arg)
{
    // TODO: Your code here.
    Simulation * s = (Simulation *)arg;
    SupplierRequestGenerator supplier_req(&(s->supplierTasks));
    supplier_req.enqueueTasks(s->maxTasks, &(s->store));
    supplier_req.enqueueStops(s->numSuppliers);
    sthread_exit(); //thread_exit
    return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * customerGenerator --
 *
 *      The customer generator thread. The argument is a pointer to
 *      the shared Simulation object.
 *
 *      Enqueue arg->maxTasks requests to the customer queue, then
 *      stop all customer threads by enqueuing arg->numCustomers
 *      stop requests.
 *
 *      Use a CustomerRequestGenerator to generate and enqueue
 *      requests.  For the fineMode argument to the constructor
 *      of CustomerRequestGenerator, use the output of
 *      store.fineModeEnabled() method, where store is a field
 *      in the Simulation class.
 *
 *      This thread should exit when done.
 *
 * Results:
 *      Does not return. Exit instead.
 *
 * ------------------------------------------------------------------
 */
static void*
customerGenerator(void* arg)
{
    // TODO: Your code here.
    Simulation * s = (Simulation *)arg;
    CustomerRequestGenerator customer_req(&(s->customerTasks), (s->store).fineModeEnabled());
    customer_req.enqueueTasks(s->maxTasks, &(s->store));
    customer_req.enqueueStops(s->numCustomers);
    sthread_exit();
    return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * supplier --
 *
 *      The main supplier thread. The argument is a pointer to the
 *      shared Simulation object.
 *
 *      Dequeue Tasks from the supplier queue and execute them.
 *
 * Results:
 *      Does not return.
 *
 * ------------------------------------------------------------------
 */
static void*
supplier(void* arg)
{
    // TODO: Your code here.
    Simulation * s = (Simulation *)arg;
    while(1)
    {
        Task t = (s->supplierTasks).dequeue();
        t.handler(t.arg); //call the handler method with t.arg as the method parameter
    }
    //return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * customer --
 *
 *      The main customer thread. The argument is a pointer to the
 *      shared Simulation object.
 *
 *      Dequeue Tasks from the customer queue and execute them.
 *
 * Results:
 *      Does not return.
 *
 * ------------------------------------------------------------------
 */
static void*
customer(void* arg)
{
    // TODO: Your code here.
    Simulation * s = (Simulation *)arg;
    while(1)
    {
        Task t = (s->customerTasks).dequeue();
        t.handler(t.arg); //call the handler method with arg as the method parameter
    }
    //return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * startSimulation --
 *      Create a new Simulation object. This object will serve as
 *      the shared state for the simulation. 
 *
 *      Create the following threads:
 *          - 1 supplier generator thread.
 *          - 1 customer generator thread.
 *          - numSuppliers supplier threads.
 *          - numCustomers customer threads.
 *
 *      After creating the worker threads, the main thread
 *      should wait until all of them exit, at which point it
 *      should return.
 *
 *      Hint: Use sthread_join.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
static void
startSimulation(int numSuppliers, int numCustomers, int maxTasks, bool useFineMode)
{
    // TODO: Your code here.
    Simulation s(useFineMode);
    s.numSuppliers = numSuppliers;
    s.numCustomers = numCustomers;
    s.maxTasks = maxTasks;

    sthread_t sup_gen_thr; //supplier generator thread
    sthread_create(&sup_gen_thr, supplierGenerator, &s);

    sthread_t cus_gen_thr; //customer generator thread
    sthread_create(&cus_gen_thr, customerGenerator, &s);

    sthread_t sup_thr_array[numSuppliers]; //supplier thread array
    for(int i = 0; i < numSuppliers; i++)
    {
        sthread_create(&(sup_thr_array[i]), supplier, &s);
    }

    sthread_t cus_thr_array[numCustomers]; //customer thread array
    for(int i = 0; i < numCustomers; i++)
    {
        sthread_create(&(cus_thr_array[i]), customer, &s);
    }
    
    for(int i = 0; i < numSuppliers; i++)
    {
        sthread_join(sup_thr_array[i]);
    }
    for(int i = 0; i < numCustomers; i++)
    {
        sthread_join(cus_thr_array[i]);
    }

    sthread_join(sup_gen_thr);
    sthread_join(cus_gen_thr);
}

int main(int argc, char **argv)
{
    bool useFineMode = false;

    // Seed the random number generator.
    // You can remove this line or set it to some constant to get deterministic
    // results, but make sure you put it back before turning in.
    srand(time(NULL));

    if (argc > 1)
        useFineMode = strcmp(argv[1], "--fine") == 0;
    startSimulation(10, 10, 100, useFineMode);
    return 0;
}

