# Rootkit 101
	Filter out the result


## Solution
In this POC, it filter out the the program name by `grep -v` by call the
original program. Then install our rootkit by replace to the original
program, move the original which give the prefix.

## Conclusion
+ Prepare our rootkit by execute the original program and filter out the result
+ Rename the original program which give some prefix
+ Install our rootkit and replace the original program

## Lack
The resoult we filter out will not display as normal program (e.g. the color
and format of rootkit `ls` are not similar with normal `ls`)
