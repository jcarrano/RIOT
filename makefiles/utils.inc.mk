# Assorted make utilities
# TODO: move tests out of here

test-all-utils: test-streq test-assert-positive test-assert_eq-positive

# Return an empty string if the arguments are different (works only for single
# words
streq = $(filter $(1),$(2))

test-streq:
	@echo empty,empty: $(call streq,,)
	@echo empty,x: $(call streq,,x)
	@echo x, empty: $(call streq,x,)
	@echo x, y: $(call streq,abc,abcw)
	@echo y, x: $(call streq,abcw,abc)
	@echo x,x: $(call streq,abcw,abcw)

# Produce an error if the argument is empty. The second argument defines the
# message.
assert = $(if $(1),,$(error $(2)))

test-assert-positive:
	@echo OK? $(call assert,x,"This should not fail")

test-assert-negative:
	@echo OK? $(call assert,,"This should fail")

# Produce an error if the first two arguments are different. The third argument
# defines the message. If the message is empty a default message is shown.
assert_eq = $(if $(call streq,$(1),$(2)),,$(error $(if $(3),$(3),Expected $(2) and got $(1))))

test-assert_eq-positive:
	@echo OK? $(call assert_eq,x,x,"This should not fail")

test-assert_eq-negative:
	@echo OK? $(call assert_eq,z,x,"This should fail")

# Get the remaining words in a list
rest = $(wordlist 2,$(words $(1)),$(1))

# Return a string if the argument is empty and vice versa
strnot = $(if $(1),,x)

# Concatenate a list of strings
concat = $(if $(1),$(firstword $(1))$(call concat,$(call rest,$(1))),)

test-concat:
	@echo OK? $(call assert_eq,$(call concat,ab xy z),abxyz,"This should not fail")

# Intercalate: concatenate words in a list with a separator between each one.
intercal = $(firstword $(2))$(call concat,$(addprefix $(1),$(call rest,$(2))))

test-intercal:
	@echo OK? $(call assert_eq,$(call intercal,-,ab xy z),ab-xy-z,)
	@echo OK? $(call assert_eq,$(call intercal,-,ab),ab,"This should not fail")

_putdots = $(patsubst %,..,$(1))
_relpath = $(if $(call streq,$(firstword $(1)),$(firstword $(2))),\
$(call _relpath,$(call rest,$(1)),$(call rest,$(2))),\
$(call _putdots,$(1)) $(2))

# Join components to make a path name
joinpath = $(call intercal,/,$(1))

# Split a path into its components
splitpath = $(subst /, ,$(1))

# Given a directory and a path (both absolute), calculate the path of the second
# argument relative to the first one.
relpath = $(call joinpath,$(call _relpath,$(call splitpath,$(1)),$(call splitpath,$(2))))

# Use coreutils to calculate a relative path
sys_relpath = $(shell realpath -m --relative-to=$(1) $(2))

test-relpath:
	@echo OK? $(call assert_eq,$(call relpath,/1/2/3/5/6/7,/1/2/3/4/5/6/7),$(call sys_relpath,/1/2/3/5/6/7,/1/2/3/4/5/6/7),)
	@echo OK? $(call assert_eq,$(call relpath,/1/2/3/5/6/7,/),$(call sys_relpath,/1/2/3/5/6/7,/),)
	@echo OK? $(call assert_eq,$(call relpath,/,/1/2/3/5/6/),$(call sys_relpath,/,/1/2/3/5/6/),)

test-relpath-negtive:
	# these fail but it should not matter because $1 should always be a dir
	# and $2 a file
	echo $(call relpath,/,/)
	echo $(call relpath,/1/2/3,/1/2/3)

# Return a path relative to the current directory
relpath_here = $(call relpath,$(abspath .),$(abspath $(1)))
