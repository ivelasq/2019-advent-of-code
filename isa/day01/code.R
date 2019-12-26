#######################
# Advent of Code 2019 #
#       Day 01        #
#######################

library(tidyverse)

# puzzle input

day1_input <-
  c(80740, 103617, 86598, 135938, 98650, 84982, 79253, 122436, 119679, 89758, 131375, 112500, 111603, 112563, 111174, 114961, 131423, 144920, 56619, 94542, 94533, 116453, 78286, 70985, 91005, 116346, 137141, 90815, 68806, 61549, 116078, 53067, 116991, 58210, 54878, 98184, 108532, 130796, 144893, 137845, 57481, 133204, 125789, 99573, 121718, 73905, 105746, 134401, 136337, 74788, 147758, 128636, 97457, 84983, 57520, 125839, 68230, 106761, 147436, 95604, 142427, 82718, 81692, 138713, 53145, 90348, 69312, 139908, 139836, 91889, 126399, 130204, 103643, 70653, 81236, 99555, 64461, 128172, 122914, 71321, 141502, 124121, 67214, 64612, 78519, 69582, 124489, 95904, 124274, 66556, 140500, 112775, 114855, 57332, 50072, 79830, 126844, 67276, 137841, 108654)

fuel_equation <- function(x) {
  floor(x / 3) - 2
}

# Part 1 ------------------------------------------------------------------

# testing using examples

ex1_input <- 
  c(12, 14, 1969, 100756)

for (i in ex1_input) {
  fuel = fuel_equation(i)
  print(fuel)
}

# part 1 sum

fuel_sum <- 0

for (i in day1_input) {
  fuel = fuel_equation(i)
  fuel_sum = fuel_sum + fuel
  print(fuel_sum)
}

# Part 2 ------------------------------------------------------------------

day1_input <-
  c(80740, 103617, 86598, 135938, 98650, 84982, 79253, 122436, 119679, 89758, 131375, 112500, 111603, 112563, 111174, 114961, 131423, 144920, 56619, 94542, 94533, 116453, 78286, 70985, 91005, 116346, 137141, 90815, 68806, 61549, 116078, 53067, 116991, 58210, 54878, 98184, 108532, 130796, 144893, 137845, 57481, 133204, 125789, 99573, 121718, 73905, 105746, 134401, 136337, 74788, 147758, 128636, 97457, 84983, 57520, 125839, 68230, 106761, 147436, 95604, 142427, 82718, 81692, 138713, 53145, 90348, 69312, 139908, 139836, 91889, 126399, 130204, 103643, 70653, 81236, 99555, 64461, 128172, 122914, 71321, 141502, 124121, 67214, 64612, 78519, 69582, 124489, 95904, 124274, 66556, 140500, 112775, 114855, 57332, 50072, 79830, 126844, 67276, 137841, 108654)

fuel_equation <- function(x) {
  floor(x / 3) - 2
}

fuel_sum <- 0

# testing it out

ex2_input <- c(1969)

for (i in ex2_input) {
  fuel = i
  repeat {
    req_fuel = fuel_equation(fuel)
    if (req_fuel > 0)
    {
      fuel_sum = (fuel_sum + req_fuel)
      fuel = req_fuel
      #add_fuel = fuel_equation(fuel)
      #fuel_sum = (fuel_sum + add_fuel)
      #fuel = add_fuel
    }
    else
    {
      break
    }
    # add_fuel = fuel_equation(fuel)
    # fuel_sum = fuel + add_fuel + fuel_sum
    # fuel = add_fuel
  }
  tot_fuel = fuel_sum
  print(tot_fuel)
}

# now for the rest

for (i in day1_input) {
  fuel = i
  repeat {
    req_fuel = fuel_equation(fuel)
    if (req_fuel > 0)
    {
      fuel_sum = (fuel_sum + req_fuel)
      fuel = req_fuel
      #add_fuel = fuel_equation(fuel)
      #fuel_sum = (fuel_sum + add_fuel)
      #fuel = add_fuel
    }
    else
    {
      break
    }
    # add_fuel = fuel_equation(fuel)
    # fuel_sum = fuel + add_fuel + fuel_sum
    # fuel = add_fuel
  }
  tot_fuel = fuel_sum
  print(tot_fuel)
}
