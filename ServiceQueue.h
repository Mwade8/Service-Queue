
#ifndef _SERVICE_QUEUE_H
#define _SERVICE_QUEUE_H

#include <iostream>
#include <vector>
#include <utility>
#include <cstdio>
using namespace std;


class ServiceQueue {

  private:
        /** Your private data members here!
        *   (you should have NO PUBLIC data members!
        *
        * Nested private types also go here.
        * For example, if your design needs some kind of
        *   structure, you would specify it here.
        */
        //Node struct
        struct Node{
          Node* next;
          Node* prev;
          int id;
          bool inUse;
          Node(int x){
              id=x;
              prev=next=nullptr;
              inUse=true;

          }
        };

        Node* front;
        Node* back;

        int size;
        Node*  unusuedBuzzFront;
        Node* unusedBuzzBack;
        vector<Node*> pBuzzBucket;

  public:

	/**
	 * Constructor
	 * Description: intializes an empty service queue.
	 *
	 * RUNTIME REQUIREMENT: O(1)
	 *
         * Finished
	 */
         ServiceQueue() {
             front = nullptr;
             back = nullptr;
             unusuedBuzzFront=nullptr;
             unusedBuzzBack=nullptr;
             size=0;

         }

	/**
	 * Destructor
	 * Description:  deallocates all memory assciated
	 *   with service queue
	 *
	 * RUNTIME REQUIREMENT:  O(N_b) where N_b is the number of buzzer
	 *	IDs that have been used during the lifetime of the
	 *	service queue; in general, at any particular instant
	 *	the actual queue length may be less than N_b.
	 *
	 *	[See discussion of "re-using buzzers" below]
	 *
         * Finished
	 */
	~ServiceQueue() {
    cout<<"Destructor called"<<endl;

    for(int i=0; i<pBuzzBucket.size();i++){
        delete pBuzzBucket[i];
    }
    
    pBuzzBucket.clear();
  }


	/**
	 * Function: snapshot()
         * param:  buzzers is an integer vector passed by ref
	 * Description:  populates buzzers vector with a "snapshot"
         *               of the queue as a sequence of buzzer IDs
         *
	 *
	 * RUNTIME REQUIREMENT:  O(N)  (where N is the current queue
	 *		length).
	 */
	void snapshot(std::vector<int> &buzzers) {
    //clear the buzzer cus u dont know where it came from
    buzzers.clear();
    Node*temp=front;
    while(temp!=nullptr){
      buzzers.push_back(temp->id);

      temp=temp->next;
    }
    delete temp;
  }

	/**
	 * Function: length()
	 * Description:  returns the current number of
	 *    entries in the queue.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	int  length() {

             return size;   // placeholder

        }

	/**
	 * Function: give_buzzer()
         * Return:   buzzer-ID (integer) assigned to the new customer.
	 * Description:  This is the "enqueue" operation.  For us
	 *    a "buzzer" is represented by an integer (starting
	 *    from zero).  The function selects an available buzzer
	 *    and places a new entry at the end of the service queue
	 *    with the selected buzer-ID.
	 *    This buzzer ID is returned.
	 *    The assigned buzzer-ID is a non-negative integer
	 *    with the following properties:
	 *
	 *       (1) the buzzer (really it's ID) is not currently
	 *         taken -- i.e., not in the queue.  (It
	 *         may have been in the queue at some previous
	 *         time -- i.e., buzzer can be re-used).
	 *	  This makes sense:  you can't give the same
	 *	  buzzer to two people!
	 *
	 *       (2) Reusable Buzzers:  A re-usable buzzer is
	 *	  a buzzer that _was_ in the queue at some previous
	 *	  time, but currently is not.
	 *
         *         REQUIREMENT:  If there is one or more reusable
         *         buzzer, you MUST return one of them; furthermore,
         *         it must be the buzzer that became reusable most
         *         MOST RECENTLY.
	 *
	 *       (3) if there are no previously-used / reusable buzzers,
         *         the SMALLEST possible buzzer-ID is used (retrieved from
         *         inventory).
	 *	    Properties in this situation (where N is the current
	 *	      queue length):
	 *
	 *		- The largest buzzer-ID used so far is N-1
	 *
	 *		- All buzzer-IDs in {0..N-1} are in the queue
	 *			(in some order).
	 *
	 *		- The next buzzer-ID (from the basement) is N.
	 *
	 *    In other words, you can always get more buzzers (from
	 *    the basement or something), but you don't fetch an
	 *    additional buzzer unless you have to (i.e., no reusable buzzers).
	 *
	 * Comments/Reminders:
	 *
	 *	Rule (3) implies that when we start from an empty queue,
	 *	the first buzzer-ID will be 0 (zero).
	 *
	 *	Rule (2) does NOT require that the _minimum_ reuseable
	 *	buzzer-ID be used.  If there are multiple reuseable buzzers,
	 *	any one of them will do.
	 *
	 *	Note the following property:  if there are no re-useable
	 *	buzzers, the queue contains all buzzers in {0..N-1} where
	 *       N is the current queue length (of course, the buzzer IDs
	 *	may be in any order.)
	 *
	 * RUNTIME REQUIREMENT:  O(1)  ON AVERAGE or "AMORTIZED"
	 *          In other words, if there have been M calls to
	 *		give_buzzer, the total time taken for those
	 *		M calls is O(M).
	 *
	 *		An individual call may therefore not be O(1) so long
	 *		as when taken as a whole they average constant time.
	 *
	 */
   //creates a new buzzer node adds it to the end of the guest list
   int getNewBuzzer(){
     Node*temp=new Node(pBuzzBucket.size());
     pBuzzBucket.push_back(temp);
     //if no nodes in queue
     if(front==nullptr){
       temp->next=nullptr;
       temp->prev=nullptr;
       front=temp;
       back=temp;
     }
     //if there is at least one node in queue
     else{
       back->next=temp;
       temp->next=nullptr;
       temp->prev=back;
       back=temp;
     }
     temp->inUse=true;
     return temp->id;
   }
   //doesnt point to node
   int popBuzzer(){
     Node*temp=unusuedBuzzFront;
     int x=unusuedBuzzFront->id;
     //if there is only one node in buzzer bucket
     if(temp->next==nullptr){
       unusuedBuzzFront=nullptr;
       unusedBuzzBack=nullptr;
       //code to add popped buzzer to end of queue

       //case if guest is nullptr
       if(front==nullptr){
         temp->next=nullptr;
         temp->prev=nullptr;
         front=temp;
         back=temp;
       }
       //if guest is not null
       else{
         back->next=temp;
         temp->next=nullptr;
         temp->prev=back;
         back=temp;
       }

     }
     //more than one node in buzzerbucket
     else{
       unusuedBuzzFront=temp->next;
       unusuedBuzzFront->prev=nullptr;

       //code to add popped buzzer to end of queue

       //case if guest is nullptr
       if(front==nullptr){
         temp->next=nullptr;
         temp->prev=nullptr;
         front=temp;
         back=temp;
       }
       //if guest is not null
       else{
         back->next=temp;
         temp->next=nullptr;
         temp->prev=back;
         back=temp;
       }


     }
     temp->inUse=true;
     return x;
   }
   //enqueue
	int  give_buzzer() {

    if(unusuedBuzzFront == nullptr){
      size++;
      return getNewBuzzer();
    }
    else{
      size++;
      return popBuzzer();

    }


  }

	/**
	 * function: seat()
	 * description:  if the queue is non-empty, it removes the first
	 *	 entry from (front of queue) and returns the
	 *	 buzzer ID.
	 *	 Note that the returned buzzer can now be re-used.
	 *
	 *	 If the queue is empty (nobody to seat), -1 is returned to
	 *	 indicate this fact.
         *
         * Returns:  buzzer ID of dequeued customer, or -1 if queue is empty.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
   //returns id and deletes guest
  int removeGuest(){
    int x;
    Node*temp=front;
    //for one node

    if(front->next==nullptr){
      front->prev=nullptr;
      back=front=nullptr;
      //x=temp->id;
      pushFrontBuzzer(temp);

    }
    //for multiple nodes
    else{
      front=front->next;
      front->prev=nullptr;
      temp->next=nullptr;
      pushFrontBuzzer(temp);
    }
    temp->inUse=false;
    return temp ->id;

  }
  //es back into the end of the list
  void pushBackBuzzer(Node*temp){
    if(front==nullptr){
      temp->next=nullptr;
      temp->prev=nullptr;
      front=temp;
      back=temp;
    }
    else{
      back->next=temp;
      temp->next=nullptr;
      temp->prev=back;
      back=temp;
    }
  }
  void pushFrontBuzzer(Node*temp){
    if (unusuedBuzzFront == nullptr){
      unusuedBuzzFront = unusedBuzzBack= temp;
      temp->next = nullptr;
      temp->prev = nullptr;
    }
    else{
      unusuedBuzzFront->prev = temp;
      temp->next = unusuedBuzzFront;
      temp->prev = nullptr;
      unusuedBuzzFront = temp;
    }
    temp->inUse=false;
  }


  //returns buzzerID of dequeued
	int seat() {
    int x;
    //if guest queue is non empty
    if (size==0){
        return -1;
    }

    else{
      x=removeGuest();

    }
    size--;
    return x;
  }


	/**
	 * function: kick_out()
	 *
	 * description:  Some times buzzer holders cause trouble and
	 *		a bouncer needs to take back their buzzer and
	 *		tell them to get lost.
	 *
	 *		Specifially:
	 *
	 *		If the buzzer given by the 2nd parameter is
	 *		in the queue, the buzzer is removed (and the
	 *		buzzer can now be re-used) and 1 (one) is
	 *		returned (indicating success).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */

	bool kick_out(int buzzer) {

    //checks if move is even logical
    if(buzzer<0 || buzzer>pBuzzBucket.size()-1){
      return false;
    }
    Node *temp=pBuzzBucket[buzzer];

    if (temp->inUse==false){
      return false;
    }
    //if only node in queue
    if(temp->prev==nullptr && temp->next==nullptr){
      front=nullptr;
      back=nullptr;
      pushFrontBuzzer(temp);

    }
    //first person in queue
    else if(temp->prev==nullptr){
      front=temp->next;
      Node*temp2=front;
      temp2->prev=nullptr;
      temp->next=nullptr;
      temp->prev=nullptr;
      pushFrontBuzzer(temp);
    }
    //last person in queue
    else if(temp->next==nullptr){
      back=temp->prev;
      Node*temp2=back;
      temp2->next=nullptr;
      temp->next=nullptr;
      temp->prev=nullptr;
      pushFrontBuzzer(temp);
    }

    //node is in the middle
    else{
      (temp->prev)->next=temp->next;
      (temp->next)->prev=temp->prev;
      temp->prev=nullptr;
      temp->next=nullptr;
      pushFrontBuzzer(temp);

    }
    temp->inUse=false;
    size--;
    return true;


  }

	/**
	 * function:  take_bribe()
	 * description:  some people just don't think the rules of everyday
	 *		life apply to them!  They always want to be at
	 *		the front of the line and don't mind bribing
	 *		a bouncer to get there.
	 *
	 *	        In terms of the function:
	 *
	 *		  - if the given buzzer is in the queue, it is
	 *		    moved from its current position to the front
	 *		    of the queue.  1 is returned indicating success
	 *		    of the operation.
	 *		  - if the buzzer is not in the queue, the queue
	 *		    is unchanged and 0 is returned (operation failed).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */

   //helper
   void pushFrontGuest(Node*temp){
     if (front == nullptr){
       front = back= temp;
       temp->next = nullptr;
       temp->prev = nullptr;
     }
     else{
       front->prev = temp;
       temp->next = front;
       temp->prev = nullptr;
       front = temp;
     }
   }

	bool take_bribe(int buzzer) {
    if(buzzer<0 || buzzer>pBuzzBucket.size()-1){
      return false;
    }
    Node *temp=pBuzzBucket[buzzer];

    if (temp->inUse==false){
      return false;
    }
    //one node in list
    if((temp->prev==nullptr) && (temp->next==nullptr)){
      front=temp;
      back=temp;
      return true;
    }
    //first person in list
    else if(temp->prev==nullptr){
      return true;
    }
    //last person in list
    else if(temp->next==nullptr){
      back=temp->prev;
      Node*temp2=back;
      temp2->next=nullptr;
      pushFrontGuest(temp);
    }
    //if in middle of the list
    else{
      (temp->prev)->next=temp->next;
      (temp->next)->prev=temp->prev;
      //testing delete if doesnt work
      temp->prev=nullptr;
      temp->next=nullptr;
      pushFrontGuest(temp);

    }
    return true;

  }



};   // end ServiceQueue class

#endif
