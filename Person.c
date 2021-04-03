#include "Person.h"
#include "Constants.h"

#define AGE_ADDITION 0.08

/**
 * Allocates dynamically new person with (at least) the following
 * input data:
 * @param id (IdT) the id of the person.
 * @param name (char *) the name of the person (received as string) (ex: "Moshe Stam-shem").
 * @param age (size_t) the age of the person.
 * @param is_sick (int) boolean value (0/1) which indicates if the person is sick.
 * @return (struct Person *) pointer to dynamically allocated person,
 * @if_fails returns NULL.
 * @assumption the input would be valid.
 */
Person *PersonAlloc(IdT id, char *name, size_t age, int is_sick){
    Person *person = malloc(sizeof(Person));
    if (person == NULL){
        return NULL;
    }
    person->name = (char*) malloc(strlen(name)+1); //@todo: free in PersonFree
    if (person->name == NULL){return  NULL;}
    strcpy(person->name, name);
    person->id = id;
    person->age = age;
    person->is_sick = is_sick;
    person->meetings = malloc(sizeof(Meeting) * PERSON_INITIAL_SIZE);
    if (person->meetings == NULL){return  NULL;}
    person->meetings_capacity = PERSON_INITIAL_SIZE;
    person->num_of_meetings = 0;
    person->infection_rate = is_sick ? 1 : (person->age > AGE_THRESHOLD ? AGE_ADDITION : 0);
    return person;
}

/**
 * Frees everything the person has allocated and the pointer itself.
 *
 * my addition to documentation:
 * free persons name, **meetings and person it self.
 * the rest is not dynamically allocated.
 *
 * @param p_person (struct Person **) pointer to dynamically allocated person.
 * @assumption you can not assume anything.
 */
void PersonFree(Person **p_person){
    if ((p_person == NULL)||(*p_person == NULL)){
        return;
    }
    free((*p_person)->name);
    free((*p_person)->meetings);
    free(*p_person);
    *p_person = NULL;
}

/**
 * Returns a pointer to the meeting with the given person (the person who has the given ID).
 * @param person (struct Person *) the person we would like to get his/her meeting.
 * @par id (IdT) the id of the person she/he met with.
 * @return (Meeting *) a pointer to meeting, returns NULL
 * if no such exists.
 * @if_fails returns NULL.
 * @assumption you can not assume anything.
 */
Meeting *PersonGetMeetingById(const Person *const person, IdT id){
    if (person == NULL){
        return NULL;
    }
    for (size_t ix = 0 ; ix < person->num_of_meetings; ix++){
        if ((person->meetings[ix]->person_1->id == id)||(person->meetings[ix]->person_2->id == id)){
            return person->meetings[ix];
        }
    }
    return NULL;
}

/**
 * simple function for modular coding reasons
 * @todo - check if p1-p2 should be 1 or -1.
 * @param diff person1- person2
 * @return needed return value for GeneralComparison between to people
 */
int GeneralComparison(double diff){
    if (diff == 0){
        return 0;
    }
    else if (diff < 0){
        return -1;
    }
    else{ //diff > 0
        return 1;
    }
}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareById(const Person *person_1, const Person *person_2){
    if(( person_1 == NULL)||(person_2 == NULL)){
        return 0;
    }
    int diff = (int) (person_1->id - person_2->id); //
    return GeneralComparison(diff);

}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareByName(const Person *person_1, const Person *person_2) {
    if ((person_1 == NULL) || (person_2 == NULL)) {
        return 0;
    }
    int diff = strcmp(person_1->name, person_2->name);
    return GeneralComparison(diff);
}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareByInfectionRate(const Person *person_1, const Person *person_2){
    if(( person_1 == NULL)||(person_2 == NULL)){
        return 0;
    }
    double diff  = person_1->infection_rate - person_2->infection_rate;
    return GeneralComparison(diff);
}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareByAge(const Person *person_1, const Person *person_2){
    if(( person_1 == NULL)||(person_2 == NULL)){
        return 0;
    }
    int age_diff  = (int)(person_1->age - person_2->age);// unsigned long to int
    return GeneralComparison(age_diff);
}


/**
 * printing for self yse and testing.
 * @param person
 */
void PrintPerson(Person *person) {
    printf("name:%s, ID:%li", person->name, person->id);
}

