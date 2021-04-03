#include "SpreaderDetector.h"

int main() {
    printf("[Pre-submission] pre-submission on its way...\n");

    SpreaderDetector *spr = SpreaderDetectorAlloc();

    Person *p1 = PersonAlloc(206399999, "Stam Shem", 53, 1);
    Person *p2 = PersonAlloc(205336312, "No Such-Name", 50, 0);
    SpreaderDetectorAddPerson(spr, p1);
    SpreaderDetectorAddPerson(spr, p2);
    size_t num_of_people = SpreaderDetectorGetNumOfPeople(spr);
    if (num_of_people != 2) {
        return 1;
    }
    printf("[Pre-submission] added Stam Shem and No Such-Name to spreader successfully.\n");

    Meeting *meeting = MeetingAlloc(p1, p2, 10.1, 10);
    SpreaderDetectorAddMeeting(spr, meeting);
    int meetings_num = SpreaderDetectorGetNumOfMeetings(spr); // returns 1
    if (meetings_num != 1) {
        return 1;
    }
    printf("[Pre-submission] added meeting successfully.\n");

    SpreaderDetectorCalculateInfectionChances(spr);
    double stam_shem_chance = SpreaderDetectorGetInfectionRateById(spr, 206399999);
    double no_such_name_chance = SpreaderDetectorGetInfectionRateById(spr, 205336312);
    int correct_calc = (stam_shem_chance == 1.) && (0 < no_such_name_chance && no_such_name_chance < 1);
    if (!correct_calc) {
        return 1;
    }
    printf("[Pre-submission] made simple chances calculation successfully.\n");
    
    PersonFree(&p1);
    PersonFree(&p2);
    MeetingFree(&meeting);
    SpreaderDetectorFree(&spr);
    printf("[Pre-submission] you passed pre-sub test successfully.\n");
    printf("[Important] No valgrind checks are done here!\n");


    return 0;
}
