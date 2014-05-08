# Rootkit 102
	Using preload library as rootkit to hide program

## Solution
The idea comes from Jynx which is a LD\_PRELOAD based userland rootkit.

## Conclusion
+ Replace the libc methods by dlopen and dlsym

## Lack
Need to setup the LD\_PRELOAD that will found by trace the system configure
