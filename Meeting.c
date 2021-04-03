#include "Meeting.h"


/**
 * when we add a new meeting we need to add it to person1->meetings.
 * we may assume that it goes only to person 1and dose not go to person2.
 * @param person
 * @param meeting
 */
void AddMeetingToPerson(Person *person, Meeting *meeting){
    if ((person == NULL) || (meeting == NULL)){return;}
    if (person->num_of_meetings == person->meetings_capacity){
        person->meetings_capacity *= PERSON_GROWTH_FACTOR;
        person->meetings = realloc(person->meetings,sizeof(Meeting) * person->meetings_capacity);
    }
    person->meetings[person->num_of_meetings] = meeting;
    person->num_of_meetings++;
}


/**
 * to save space, if we are to delete a single meeting, we would like to free (done in MeetingFree)
 * and remove it from the person list of meetings
 * no need to check, its an inner function for developer use only.
 * no need to check if meeting in person->meetings, will only be called from MeetingFree
 * @param person
 * @param meeting
 */
void RemoveMeetingFromPerson(Person *person, Meeting *meeting){
    //better to run top down because it matches what PersonFree does.
    size_t ix = person->num_of_meetings;
    if (ix == 0){return;}
    for(; ix > 0; ix-- ){
        if (person->meetings[ix - 1] == meeting){
            ix--;
            break;
        }
    }
    for (; ix < person->num_of_meetings-1; ix++) {
        person->meetings[ix] = person->meetings[ix+1];
    }
    person->num_of_meetings--;
    if ((person->meetings_capacity > PERSON_INITIAL_SIZE)&&((person->meetings_capacity)/PERSON_GROWTH_FACTOR > person->num_of_meetings)){
        //to save un used space.
        person->meetings_capacity /= PERSON_GROWTH_FACTOR;
        person->meetings = realloc(person->meetings, sizeof(Meeting) * person->meetings_capacity);
    }
}

/**
 * Allocating (dynamically) new meeting with (at least) the following
 * input data:
 * @param person_1 (struct Person *) pointer to the first person in the meeting.
 * @param person_2 (struct Person *) pointer to the second person in the meeting.
 * @param measure (double) the time of the meeting in minutes.
 * @param distance (double) the distance the two people where in.
 * @return (struct Meeting *) pointer to dynamically allocated meeting.
 * @if_fails returns NULL.
 * @assumption the inputs would be valid.
 */
Meeting *MeetingAlloc(Person *person_1, Person *person_2, double measure, double distance){
    if(( person_1 == NULL)||(person_2 == NULL)){
        return NULL;
    }
    Meeting *meeting = malloc(sizeof(Meeting));
    if (meeting == NULL){
        return NULL;
    }
    meeting->person_1 = person_1;
    meeting-> person_2 = person_2;
    meeting->measure = measure;
    meeting->distance = distance;
    AddMeetingToPerson(person_1,meeting);//ex3 update forum states that it shall only belong to p1.
    return  meeting;
}

/**
 * Frees everything the meeting has allocated and the pointer itself.
 * @param p_meeting (struct Meeting **) pointer to dynamically allocated meeting.
 * @assumption you can not assume anything.
 */
void MeetingFree(Meeting **p_meeting){
    if ((p_meeting ==NULL) ||(*p_meeting == NULL)){
        return;
    }
    (*p_meeting)->person_1 = NULL;
    free(*p_meeting);
    *p_meeting = NULL;
}

/**
 * Returns a pointer to one of the persons in the meeting.
 * @param meeting (struct Meeting *) the meeting we would like to
 * get its person.
 * @param person_ind (size_t) the index of the person we
 * want (can be either 1 or 2).
 * @return (struct Person *) pointer to the person we want.
 * person_ind == 1 ==> return person_1 (according the person_1 given in
 * MeetingAlloc).
 * person_ind == 2 ==> return person_2 (according to the person_2 given in
 * MeetingAlloc).
 * @if_failds return NULL
 * @assumption you can not assume anything.
 */
Person *MeetingGetPerson(const Meeting * const meeting, size_t person_ind){
    if (meeting == NULL){return NULL;}
    //if wanted person is null we will still return null.
    if (person_ind == 1){
        return meeting->person_1;
    } else if (person_ind == 2){
        return meeting->person_2;
    }
    return NULL; // incase ind != 1||2
}

