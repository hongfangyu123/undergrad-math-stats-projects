# Course grade manager (C)

A menu-driven command-line program that records courses, their assessment items and weights, and the grades obtained so far. It then works out the lowest final-exam score needed to reach a target course grade, and flags courses at risk. Written in C with structs and dynamic memory (`malloc`/`realloc`). [`sample_courses.txt`](sample_courses.txt) is a sample course list used for testing.

```
gcc grade_manager.c -o grade_manager
./grade_manager
```
