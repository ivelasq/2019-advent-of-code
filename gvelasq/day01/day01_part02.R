# day 1, part 2

# setup
# if (!require(datapasta)) install.packages("datapasta")
# if (!require(purrr)) install.packages("purrr")

# copied from https://adventofcode.com/2019/day/1/input
# data <- datapasta::vector_paste()
data <- c(92349, 57040, 64079, 121555, 143735, 64642, 104858, 144446, 88871, 62338, 113424, 59960, 53999, 86867, 67224, 124130, 108921, 130492, 120361, 74426, 70397, 88106, 125442, 74237, 137818, 66633, 71756, 143276, 143456, 135698, 121124, 67739, 112861, 78572, 73565, 111899, 57543, 130314, 121605, 121426, 117143, 129957, 98042, 104760, 144846, 131238, 101076, 53328, 83592, 104077, 101952, 54137, 115363, 60556, 133086, 113361, 117829, 75003, 93729, 140022, 126219, 59907, 140589, 91812, 50485, 56232, 92858, 106820, 123423, 98553, 135315, 95583, 72278, 98702, 55709, 146773, 89719, 134752, 79562, 70455, 88468, 139824, 138646, 117516, 123267, 113754, 120353, 139145, 53219, 63053, 131434, 91705, 53650, 145234, 78461, 119587, 108976, 113613, 121790, 120366)
data <- as.integer(data)

# function to calculate fuel for fuel
calculate_fuel_fuel <- function (fuel_mass) {
  if (!is.integer(fuel_mass)) {
    stop("fuel mass must be provided in integer values")
  }
  fuel_fuel <- as.integer(floor(fuel_mass / 3L) - 2L)
  invisible(fuel_fuel)
}

# function to calculate fuel for each module
calculate_module_fuel <- function (module_mass) {
  if (!is.integer(module_mass)) {
    stop("module mass must be provided in integer values")
  }
  module_fuel <- as.integer(floor(module_mass / 3L) - 2L)
  fuel_fuel <- calculate_fuel_fuel(module_fuel)
  while (fuel_fuel > 0L) {
    module_fuel <- module_fuel + fuel_fuel
    fuel_fuel <- calculate_fuel_fuel(fuel_fuel)
  }
  invisible(module_fuel)
}

# test using examples provided
(calculate_module_fuel(14L))     # 2
(calculate_module_fuel(1969L))   # 966
(calculate_module_fuel(100756L)) # 50346

# calculate fuel for each module
module_fuel <- purrr::map_int(data, calculate_module_fuel)

# sum all module fuels to calculate total fuel
total_fuel <- purrr::reduce(module_fuel, `+`)
total_fuel # 5055835