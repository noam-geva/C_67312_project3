#include "SpreaderDetector.h"

#define AGE_ADDITION 0.08
#define DEC_FOR_STRTOL 10


// sentinel value to signal meeting to ignore, rather than failed alloc for AddMeeting
Meeting meeting_to_ignore_struct;
Meeting *meeting_to_ignore= &meeting_to_ignore_struct;

/**
 * will calculate person_2 subtree infection_rate
 * we may assume there will be no circles in the tree, ergo it will reach the leaves and stop.
 * @param meeting
 */
void RecursiveMeetingHelper(Meeting * meeting);


/**
 * Allocates (dynamically) a spreader detector with no meetings and
 * no people.
 * @return pointer to dynamically allocated SpreaderDetector struct.
 * @if_fails returns NULL.
 * @assumption you can not assume anything.
 */
SpreaderDetector *SpreaderDetectorAlloc(){
    SpreaderDetector *sd = malloc(sizeof(SpreaderDetector));
    if (sd == NULL){return NULL;}
    sd->people =malloc(sizeof(Person) * SPREADER_DETECTOR_INITIAL_SIZE);
    sd->people_size = 0;
    sd->people_cap = SPREADER_DETECTOR_INITIAL_SIZE;
    sd->meetings = malloc(sizeof(Meeting) * SPREADER_DETECTOR_INITIAL_SIZE);
    sd->meeting_size = 0;
    sd->meeting_cap = SPREADER_DETECTOR_INITIAL_SIZE;
    return sd;
}

/**
 * Frees the given spreader detector.
 * no need to decrement people/meetings size because its irrelevant at this point of time
 * @param p_spreader_detector pointer to spreader detector pointer
 * should be freed.
 * @assumption you can not assume anything.
 *
 *
 * my note about free:
 * theres some thing problemetic with the API.
 * you see, when you alloc everythings with the file then SpreaderDetctor should free its inner list
 * if you alloc something e.g. person manually then the user needs to free it using PersonFree.
 * the problem is that PersonFree cannot take SpreaderDetector as an argument and so it cannot change
 * the SD pointer to its person even though we already freed it and, then when we want to free SD
 * we will have a segmentation fault.
 * so in conclusion, i allow my self to assume that you will free each person and meeting
 * regardless of spreaderdetector.
 */
void SpreaderDetectorFree(SpreaderDetector **p_spreader_detector){
    if ((p_spreader_detector == NULL)||(*p_spreader_detector == NULL)){
        return;
    }
    free((*p_spreader_detector)->meetings);
    free((*p_spreader_detector)->people);
    free(*p_spreader_detector);
    *p_spreader_detector = NULL;
}

/**
 * checks if the person is already in SD
 * @param spreader_detector
 * @param person
 * @return 1 if true, 0 if false.
 *
 * P.S. used only in add person so no need to check if SD||person == NULL
 */
int CheckIfPersonInSpreaderDetector(SpreaderDetector *spreader_detector, Person *person){
    for (size_t ix = 0; ix < spreader_detector->people_size; ix ++){
        if (!PersonCompareById(spreader_detector->people[ix], person)){ //returns 0 if the same, so checking !output.
            return  1;
        }
    }
    return 0;
}

/**
 * Adds the given person to the spreader detector.
 * Important - each person is unique (compare by id).
 * @param spreader_detector the spreader detector we wants to add the person to.
 * @param person the person we wants to add to the spreader detector.
 * @return 1 if the person was added successfully, 0 otherwise.
 * @if_fails returns 0.
 * @assumption you can not assume anything.
 */
int SpreaderDetectorAddPerson(SpreaderDetector *spreader_detector, Person *person){
    if ((spreader_detector == NULL) || (person == NULL)){
        SpreaderDetectorFree(&spreader_detector);
        PersonFree(&person);
        return 0;
    }
    if (CheckIfPersonInSpreaderDetector(spreader_detector, person) == 1){
        return 0;
    } // from now we now that the person is not in SD.people
    if (spreader_detector->people_size == spreader_detector->people_cap){
        spreader_detector->people_cap *= SPREADER_DETECTOR_GROWTH_FACTOR;
        spreader_detector->people = realloc(spreader_detector->people,sizeof(Person) * spreader_detector->people_cap);
        if (spreader_detector->people == NULL){
            SpreaderDetectorFree(&spreader_detector);
            PersonFree(&person);
            return 0;
        }
    }
    spreader_detector->people[spreader_detector->people_size] = person;
    spreader_detector->people_size++;
    return 1;
}

/**
 * checks if the meeting is already in SD
 * @param spreader_detector
 * @param meeting
 * @return 1 if true, 0 if false.
 *
 * P.S. used only in add meeting so no need to check if SD||meeting == NULL
 */
int CheckIfMeetingInSpreaderDetector(SpreaderDetector *spreader_detector, Meeting *meeting){
        for (size_t ix = 0; ix < spreader_detector->meeting_size; ix++) {
            if (spreader_detector->meetings[ix] == meeting) { //returns 0 if the same, so checking !output.
                return 1;
            }
        }
        return 0;  // from now we now that the meeting is not in SD.meeting
}

/**
 * Adds the given meeting to the spreader detector.
 * Important - the people in the meeting should exist in the spreader detector.
 * @param spreader_detector the spreader detector we wants to add the meeting to.
 * @param meeting the meeting we wants to add to the spreader detector.
 * @return 1 if the meeting was added successfully, 0 otherwise.
 * @if_fails returns 0.
 * @assumption you can not assume anything.
 */
int SpreaderDetectorAddMeeting(SpreaderDetector *spreader_detector, Meeting *meeting){
    if ((spreader_detector == NULL) || (meeting == NULL)){
        SpreaderDetectorFree(&spreader_detector);
        MeetingFree(&meeting);
        return 0;
    }
    if (CheckIfMeetingInSpreaderDetector(spreader_detector, meeting) == 1){ return 0;}
    if (spreader_detector->meeting_size == spreader_detector->meeting_cap){
        spreader_detector->meeting_cap *= SPREADER_DETECTOR_GROWTH_FACTOR;
        spreader_detector->meetings = realloc(spreader_detector->meetings,sizeof(Meeting) * spreader_detector->meeting_cap);
        if (spreader_detector->meetings == NULL){
            SpreaderDetectorFree(&spreader_detector);
            MeetingFree(&meeting);
            return 0;
        }
    }
    spreader_detector->meetings[spreader_detector->meeting_size] = meeting;
    spreader_detector->meeting_size++;
    if (meeting->person_1->infection_rate){
        RecursiveMeetingHelper(meeting);
    }
    return 1;
}

/**
 *  find person in spreaderdetector.people using id
 * @param SD
 * @param id
 * @return pointer to said person
 */
Person *GetPersonByID(SpreaderDetector *SD,IdT id ){
    if (SD == NULL){
        return NULL;
    }
    for (size_t ix = 0; ix <SD->people_size ; ix++){
        if (SD->people[ix]->id == id){
            return SD->people[ix];
        }
    }
    return NULL;
}

/**
 * helper func for SpreaderDetectorReadMeetingsFile
 * if fails: free everything and return null
 * @return pointer to a meeting
 */
Meeting *MeetingsParser(char *line, SpreaderDetector *sd){
    char * token = strtok(line, " ");
    IdT person1_id = strtol(token,NULL, DEC_FOR_STRTOL);
    Person * person1 = GetPersonByID(sd, person1_id);
    token = strtok(NULL, " ");
    IdT person2_id = strtol(token,NULL, DEC_FOR_STRTOL);
    Person * person2 = GetPersonByID(sd, person2_id);
    token = strtok(NULL, " ");
    double distance = strtod(token,NULL);
    token = strtok(NULL, " ");
    double measure = strtod(token,NULL);
    if ((person1 != NULL) && (person2 != NULL)) {
        Meeting *meeting = MeetingAlloc(person1, person2, measure, distance);
        if (meeting == NULL) {
            SpreaderDetectorFree(&sd);
            MeetingFree(&meeting);
            return NULL;
        }
        return meeting;
    }
    return meeting_to_ignore;
}

/**
 * This function reads the file of the meeting, parses to file into meetings,
 * and inserts it to the spreader detector.
 * @param spreader_detector the spreader detector we wants to read the meetings into.
 * @param path the path to the meetings file.
 * @assumption you can assume that the path to the file is ok (and anything but that).
 */
void SpreaderDetectorReadMeetingsFile(SpreaderDetector *spreader_detector, const char *path){
    FILE *meeting_file = fopen(path, "r");
    char curr_line[MAX_LEN_OF_LINE+1];
    int flag;
    Meeting *curr_meeting = NULL;
    while (fgets(curr_line,MAX_LEN_OF_LINE+1, meeting_file)){
        curr_meeting = MeetingsParser(curr_line,spreader_detector);
        if (curr_meeting != meeting_to_ignore){
            flag = SpreaderDetectorAddMeeting(spreader_detector, curr_meeting);
            if (flag == 0){ //unsuccessful adding
                break;
            }
        }
    }
    fclose(meeting_file);
}

/**
 * similar to MeetingParser
 * @param line
 * @param sd
 * @return
 */
Person *PeopleParser(char *line, struct SpreaderDetector *sd){
    if (sd == NULL){return NULL;}
    char *token;
    token = strtok(line, " ");
    char* name = token;
    token = strtok(NULL, " ");
    IdT id = strtol(token,NULL,DEC_FOR_STRTOL);
    if (id == 0 && (strcmp(token,"0") != 0)){
            return NULL; //strtol fail
    }
    token = strtok(NULL, " ");
    unsigned int age = strtol(token,NULL,DEC_FOR_STRTOL);
    if (age == 0 && (strcmp(token,"0")!=0)){
        return NULL; //strtol fail
    }
    token = strtok(NULL, " ");
    int is_sick = 0;
    if ((token != NULL)&&(!strcmp(token, "SICK\n"))){
        is_sick = 1;
    }
    Person  *person = PersonAlloc(id,name,age,is_sick);
    if (person == NULL){
        SpreaderDetectorFree(&sd);
        PersonFree(&person);
    }
    return person;
}

/**
 * This function reads the file of the people, parses to file into person objects,
 * and inserts it to the spreader detector.
 * @param spreader_detector the spreader detector we wants to read the people into.
 * @param path the path to the people file.
 * @assumption you can assume that the path to the file is ok (and anything but that).
 */
void SpreaderDetectorReadPeopleFile(SpreaderDetector *spreader_detector, const char *path){
    FILE *people_file = fopen(path, "r");
    char curr_line[MAX_LEN_OF_LINE + 1];
    int flag;
    Person *curr_person = NULL;
    while (fgets(curr_line,MAX_LEN_OF_LINE + 1,people_file)){
        curr_person = PeopleParser(curr_line,spreader_detector);
        if (CheckIfPersonInSpreaderDetector(spreader_detector, curr_person) != 1) {
            flag = SpreaderDetectorAddPerson(spreader_detector, curr_person);
            if (flag == 0) {
                break;
            }
        } else {
            PersonFree(&curr_person);
        }
    }
    fclose(people_file);
}

/**
 * Returns the infection rate of the person with the given id.
 * @param spreader_detector the spreader detector contains the person.
 * @param id the id of the person we are looking for.
 * @return the infection rate of the person, if not person exists -
 * returns -1.
 * @if_fails returns -1.
 * @assumption you can assume anything.
 */
double SpreaderDetectorGetInfectionRateById(SpreaderDetector *spreader_detector, IdT id){
    if (spreader_detector == NULL){return -1;}
    Person *person = GetPersonByID(spreader_detector,id);
    if (person ==NULL){return -1;}
    return person->infection_rate;
}

/**
 * do i really need to explain? jk
 * simple helper func that calculates a single cnra
 * we may assume that (meeting->distance * MAX_MEASURE) != 0.
 * @param meeting
 * @return cnra
 */
double Cnra(Meeting *meeting ){
    return (meeting->measure * MIN_DISTANCE)/(meeting->distance * MAX_MEASURE);
}

/**
 * it will stop because we may assume that there are no circles in the tree.
 * @param person
 */
void RecursivePersonHelper(Person *person){
    if ( person->infection_rate == 0){return;}
    for (size_t ix = 0; ix < person->num_of_meetings; ix++){
        RecursiveMeetingHelper(person->meetings[ix]);
    }
}

/**
 * documentation at the declaration at the top of the code.
 * @param meeting
 */
void RecursiveMeetingHelper(Meeting *meeting){
    if (meeting->person_1->infection_rate == 0){return;}
    Person *person2;
    person2 = MeetingGetPerson(meeting, 2);
    if (person2 == NULL){ return;}
    person2->infection_rate = meeting->person_1->infection_rate * Cnra(meeting);
    if (person2->age > AGE_THRESHOLD){
        person2->infection_rate += AGE_ADDITION;
        if (person2->infection_rate > 1){// in case of over flow
            person2->infection_rate = 1;
        }
    }
    RecursivePersonHelper(person2);
}

/**
 * This function runs the algorithm which calculates the infection rates of the people.
 * When this algorithm ends, the user should be able to use the function
 * SpreaderDetectorGetInfectionRateById and get the infection rate of each person.
 * @param spreader_detector a spreader_detector.
 * @assumption you can not assume anything.
 */
void SpreaderDetectorCalculateInfectionChances(SpreaderDetector *spreader_detector){
    // already  dynamically calculated in the adding of every meeting.
    if (spreader_detector->people_size){return;}
    // im getting a compilation warning for not using the argument in this function.
    // and i dont want to lose points over it.
}

/**
 * Gets the recommendation for treatment for all people based on the parameters above,
 * and prints it to the given file path.
 * @param spreader_detector the spreader detector contains the person.
 * @param file_path the path to the output file.
 * @return returns 1 if printed successfully, 0 otherwise.
 * @if_fails return 0.
 * @assumption you can assume that the path to the file is ok (and anything but that).
 */
int SpreaderDetectorPrintRecommendTreatmentToAll(SpreaderDetector *spreader_detector, const char *file_path){
    if (spreader_detector == NULL){
        return 0;
    }
    FILE *outfile = fopen(file_path, "w");
    if (outfile == NULL){return 0;}
    Person *curr;
    for(size_t ix =0 ; ix < spreader_detector->people_size ; ix++){
        curr = spreader_detector->people[ix];
        if (curr->infection_rate > MEDICAL_SUPERVISION_THRESHOLD){
            fprintf(outfile, MEDICAL_SUPERVISION_THRESHOLD_MSG,curr->name, curr->id, curr->age, curr->infection_rate);
        } else if(curr->infection_rate > REGULAR_QUARANTINE_THRESHOLD){
            fprintf(outfile, REGULAR_QUARANTINE_MSG ,curr->name, curr->id, curr->age, curr->infection_rate);
        } else {
            fprintf(outfile, CLEAN_MSG ,curr->name, curr->id, curr->age, curr->infection_rate);
        }
    }
    return 1;
}

/**
 * Returns the number of people which are in the spreader detector.
 * @param spreader_detector the spreader detector object.
 * @return the number of people.
 * @if_fails return 0.
 * @assumption you can not assume anything.
 * @note 0 might represent spreader with no people, and fail both.
 * make sure you don't trust the output of the function.
 */
size_t SpreaderDetectorGetNumOfPeople(SpreaderDetector *spreader_detector){
    if (spreader_detector == NULL){return 0;}
    return spreader_detector->people_size;
}

/**
 * Returns the number of meetings which are in the spreader detector.
 * @param spreader_detector the spreader detector object.
 * @return the number of meetings.
 * @if_fails return 0.
 * @assumption you can not assume anything.
 * @note 0 might represent spreader with no meetings, and fail both.
 * make sure you don't trust the output of the function.
 */
size_t SpreaderDetectorGetNumOfMeetings(SpreaderDetector *spreader_detector){
    if (spreader_detector == NULL){return 0;}
    return spreader_detector->meeting_size;
}
