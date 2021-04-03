general notes- 
modular prog: add an efficient way to allocate and free. (maybe a macro?)

for later: realize when i need to free everything. (asaide from freeing func which will be used for it)

when initiallizeing a struct should i malloc(sizeof(struct)) || malloc(STRUCT_INITIAL_SIZE)
(e.g. person) malloc(sizeof(Person)) || malloc(PERSON_INITIAL_SIZE)

unlike ex2 no clear course of action to couldnt allocate
add macro for COULDNTALLOCATE

theres no age_addition constant in constant.h

should i start the person->meeting cap with PERSON_INITIAL_SIZE

SpreaderDetectorAddMeeting what should i do if meeting already in SD?

when should i sort the people by id and start binery sort

theres alot of:
	if (ptr == NULL) {return;}
	even though some of them will not fail for sure.
	its good practice to keep them.
	for allocating it is still a must


change every id to IdT


rename recursivemeetinghelper



Person.c -
 
modular prog:
all of the compare func are the same

in free, only name and meetings are to be free (with person it self ofcourese)
in all of the compare functions:
	what should come first? the bigger number or smaller?

	
	
Meeting.c - 

MeetingFree:
what should i free instead of just the pointer it self?

MeetingGetPerson:
what can fail should all the meeting pointer be valid?




SpreaderDetector.c -

in spreaderdetectorfree:
im freeing the meetings before the people becuase in free meeting in still need the people of said meetings.

in SpreaderDetectorAddPerson:
	if the person is already in SD.people (checking using id comperisonn)
	should realloc fail. am i to free anything?
	
 in SpreaderDetectorAddMeeting:
	if P1 infection rate > 0:
	calculate the infection rate of P2 and recrusivly its sub_tree.
	
in SpreaderDetectorReadMeetingsFile:
	will the fact that strtok goes once in SpreaderDetectorReadMeetingsFile and then once in  MeetingParser 
	will be a problem?
	
in MeetingParser and in PersonParser: 
	check what happens if strtol fails?  

in recursive helper:
	+0.08 or *1.08 change due to age. 
	end_case: >= AGE_THRESHOLD || >AGE_THRESHOLD
	

	
	





debug sess:
No.1 :
	need to fix compare functions (esspacielly by id)

No.2:
	first sess might conclusion might be off.
	trying to figure out if theres a problem with - SpreaderDetectorAddPerson
	


