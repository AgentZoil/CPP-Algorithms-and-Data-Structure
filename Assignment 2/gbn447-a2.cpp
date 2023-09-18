/*
Student Number: 7547821
Student Name: Gia Bach Nhu
Assignment 2
CSCI203
*/


#include <iostream>
#include <fstream>
using namespace std;

enum eventType {
    customer_arrival = 0, service_completion
};

struct event {
    eventType type;
    float time;
    float arrival;
    float serviceDuration;
    float enteredQueue;
    int priority;
    float queueDuration;
    int assignedServer;
};

//Arrival queue implemented as a max heap
class ArrivalQueue {
    static const int MAX_QUEUE = 500; //Max queue size
    event arrivals[MAX_QUEUE];
    //Number of customers in the queue
    int queueLength;
    //Maximum number customers ever in the queue
    int queueMaxLength;
    void siftUp(int);
    void siftDown(int);
public:
    void initialize();
    void addQueue(event);
    event removeQueue();
    bool isQueueEmpty();
    int maxQueueLength();
    void showQueue();
};

void ArrivalQueue::initialize() {
    queueLength = 0;
    queueMaxLength = 0;
}

void ArrivalQueue::addQueue(event ev) {
    queueLength < MAX_QUEUE;
    arrivals[queueLength] = ev;
    ++queueLength;
    siftUp(queueLength - 1);
    queueMaxLength = max(queueMaxLength, queueLength);
}

event ArrivalQueue::removeQueue() {
    event ev;
    queueLength > 0;
    ev = arrivals[0];
    arrivals[0] = arrivals[queueLength - 1];
    --queueLength;
    siftDown(0);
    return ev;
}

bool ArrivalQueue::isQueueEmpty() {
    return (queueLength == 0);
}

int ArrivalQueue::maxQueueLength() {
    return queueMaxLength;
}

void ArrivalQueue::siftUp(int current) {
    if (current == 0) {
        return;
    }
    int p = (current - 1) / 2;
    if (arrivals[p].priority > arrivals[current].priority) {
        return;
    } else if (arrivals[p].priority == arrivals[current].priority) {
        if (arrivals[p].arrival > arrivals[current].arrival) {
            event temp = arrivals[current];
            arrivals[current] = arrivals[p];
            arrivals[p] = temp;
            siftUp(p);
        }
    } else {
        event temp = arrivals[current];
        arrivals[current] = arrivals[p];
        arrivals[p] = temp;
        siftUp(p);
    }
}

void ArrivalQueue::siftDown(int current) {
    int child = 2 * current + 1;
    if (child >= queueLength)
        return;

    //Find the smallest child for comparison with parent
    if ((child + 1) < queueLength) {
        if (arrivals[child].priority < arrivals[child + 1].priority) {
            child++;
        } else if (arrivals[child].priority == arrivals[child + 1].priority) {
            if (arrivals[child].arrival > arrivals[child + 1].arrival) {
                child++;
            }
        }
    }
    if (arrivals[current].priority < arrivals[child].priority) {
        event temp = arrivals[current];
        arrivals[current] = arrivals[child];
        arrivals[child] = temp;
        siftDown(child);
    } else if (arrivals[current].priority == arrivals[child].priority) {
        if (arrivals[current].arrival > arrivals[child].arrival) {
            event temp = arrivals[current];
            arrivals[current] = arrivals[child];
            arrivals[child] = temp;
            siftDown(child);
        }
    }
}

//Event queue implemented as a min heap
class EventQueue {
    event *events;
    //Number of events in the queue
    int actualE;
    //Expected number of active events
    int expectedE;
    void siftDown(int);
    void siftUp(int);
public:
    void initialize(int);
    void addE(event);
    event removeE();
    eventType getNextEventType();
    bool isEmpty();
    void showEventQueue();
};

void EventQueue::initialize(int expected) {
    expectedE = expected;
    events = new event[expectedE];
    actualE = 0;
}

void EventQueue::addE(event ev) {
    actualE < expectedE;
    events[actualE] = ev;
    ++actualE;
    siftUp(actualE - 1);
}

event EventQueue::removeE() {
    event ev;
    actualE > 0;
    ev = events[0];
    events[0] = events[actualE - 1];
    --actualE;
    siftDown(0);
    return ev;
}

bool EventQueue::isEmpty() {
    return (actualE == 0);
}

eventType EventQueue::getNextEventType() {
    return events[0].type;
}

void EventQueue::siftUp(int current) {
    if (current == 0) {
        return;
    }
    int p = (current - 1) / 2;
    if (events[p].time < events[current].time) {
        return;
    } else {
        event temp = events[current];
        events[current] = events[p];
        events[p] = temp;
        siftUp(p);
    }
}

void EventQueue::siftDown(int current) {
    int child = 2 * current + 1;
    if (child >= actualE)
        return;
    if ((child + 1) < actualE) {
        if (events[child].time > events[child + 1].time) {
            child++;
        }
    }
    if (events[current].time > events[child].time) {
        event temp = events[current];
        events[current] = events[child];
        events[child] = temp;
        siftDown(child);
    }
}

class Teller {
    enum status {
        busy, idle
    };
    struct state {
        //idle or busy
        status current;
        //Number of customers served
        int served;
        // idle time
        float idle;
        float lastFinish;
    };
    state *servers;
    int serverCount;
    int serversBusy;
    int nextServer;

public:
    void initialize(int);
    int assign(float, float);
    void release(int);
    bool isIdleServer();
    int numServed(int);
    float idleTime(int);
    void showServerPool();
};

void Teller::initialize(int serverC) {
    serverCount = serverC;
    servers = new state[serverCount];
    serversBusy = 0;
    nextServer = 0;
    for (int i = 0; i < serverCount; i++) {
        servers[i].current = idle;
        servers[i].served = 0;
        servers[i].idle = 0.0;
        servers[i].lastFinish = 0.0;
    }
}

int Teller::assign(float thisStart, float thisDuration) {
    int assignedServer;
    serversBusy < serverCount;
    for (int i = 0; i < serverCount; ++i) {
        if (servers[nextServer].current == idle) {
            assignedServer = nextServer;
            break;
        } else {
            nextServer = (nextServer + 1) % serverCount;
        }
    }
    ++servers[nextServer].served;
    servers[nextServer].idle = servers[nextServer].idle + (thisStart - servers[nextServer].lastFinish);
    servers[nextServer].lastFinish = thisStart + thisDuration;
    servers[nextServer].current = busy;
    ++serversBusy;
    nextServer = (nextServer + 1) % serverCount;
    return assignedServer;
}

void Teller::release(int assignedServer) {
    serversBusy > 0;
    servers[assignedServer].current = idle;
    --serversBusy;
}

bool Teller::isIdleServer() {
    return (serversBusy != serverCount);
}

int Teller::numServed(int serverId) {
    return servers[serverId].served;
}

float Teller::idleTime(int serverId) {
    return servers[serverId].idle;
}


event createEvent(float arrival, float duration, int priority, eventType type) {
    event ev;
    ev.time = arrival;
    ev.arrival = arrival;
    ev.serviceDuration = duration;
    ev.priority = priority;
    ev.queueDuration = 0.0;
    ev.type = type;
    return ev;
}

int main() {
    char filename[20];
    ifstream fin;
    int serverC;
    float arrivalTime;
    float serviceTime;
    int priority;
    bool moreEvents;
    bool moreCustomers = true;
    ArrivalQueue arrivalQ;
    EventQueue eventQ;
    Teller tellerP;
    event ev;
    int customerCount = 0;
    float totalServiceTime = 0.0;
    float totalQueueingTime = 0.0;
    float sim_time = 0.0;

    cerr << "Enter the number of tellers: ";
    cin >> serverC;
    cerr << "Enter the name of the input file: ";
    cin >> filename;
    fin.open(filename);
    if (!fin) {
        cerr << "Error opening file " << filename << ". Program exit." << endl;
        return 0;
    }

    cout << "\nRead Inputs" << endl;
    cout << "Number of tellers: " << serverC << endl;
    cout << "Name of input file: " << filename << endl << endl;

    arrivalQ.initialize();
    eventQ.initialize(serverC + 1);
    tellerP.initialize(serverC);

    fin >> arrivalTime >> serviceTime >> priority;
    ++customerCount;

    eventQ.addE(createEvent(arrivalTime, serviceTime, priority, customer_arrival));

    moreEvents = moreCustomers or !eventQ.isEmpty();

    // Main simulation loop
    while (moreEvents) {
        //Handling service completion events
        if (eventQ.getNextEventType() == service_completion) {
            ev = eventQ.removeE();
            sim_time = ev.time;
            totalServiceTime = totalServiceTime + ev.serviceDuration;
            totalQueueingTime = totalQueueingTime + ev.queueDuration;
            tellerP.release(ev.assignedServer);
            if (!arrivalQ.isQueueEmpty()) {
                ev = arrivalQ.removeQueue();
                ev.type = service_completion;
                ev.time = sim_time + ev.serviceDuration;
                ev.queueDuration = sim_time - ev.enteredQueue;
                ev.assignedServer = tellerP.assign(sim_time, ev.serviceDuration);
                eventQ.addE(ev);
            }
        } // Handling customer arrival events
        else if (eventQ.getNextEventType() == customer_arrival) {
            ev = eventQ.removeE();
            sim_time = ev.time;
            ev.enteredQueue = sim_time;
            arrivalQ.addQueue(ev);
            if (tellerP.isIdleServer()) {
                ev = arrivalQ.removeQueue();
                ev.type = service_completion;
                ev.time = sim_time + ev.serviceDuration;
                ev.queueDuration = sim_time - ev.enteredQueue;
                ev.assignedServer = tellerP.assign(sim_time, ev.serviceDuration);
                eventQ.addE(ev);
            }
            fin >> arrivalTime >> serviceTime >> priority;
            if (moreCustomers) {
                if ((arrivalTime > 0.0) or (serviceTime > 0.0)) {
                    ++customerCount;
                    eventQ.addE(createEvent(arrivalTime, serviceTime, priority, customer_arrival));
                } else {
                    moreCustomers = false;
                }
            }
        }
        moreEvents = moreCustomers or !eventQ.isEmpty();
    }

    cout << endl;
    cout << "The statistics of the services:" << endl;
    for (int i = 0; i < serverC; ++i) {
        cout << "Number of customers served by Teller " << i + 1 <<": "<< tellerP.numServed(i) << endl;
    }
    cout << "\n";
    cout << "Total time of simulation: " << sim_time << "\n"<< endl;
    cout << "Average service time per customer: " << (totalServiceTime / customerCount) << "\n"<< endl;
    cout << "Average waiting time per customer: " << (totalQueueingTime / customerCount) << "\n"<< endl;
    cout << "The maximum length of the queue: " << arrivalQ.maxQueueLength() << "\n"<< endl;
    cout << "The average length of the queue: " << (totalQueueingTime / sim_time)<< "\n" << endl;
    for (int i = 0; i < serverC; ++i) {
        cout << "The idle rate of Teller " << i + 1 << ": " << (tellerP.idleTime(i) / sim_time) << endl;
    }
    fin.close();
    return 0;
}
