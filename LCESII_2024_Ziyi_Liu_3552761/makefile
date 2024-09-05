####
# Our project-structure/-architecture, embodied via folders.
##
SOURCES_DIR       = src
HEADERS_DIR       = inc
OBJECTS_DIR       = obj
ELF_DIR           = elf
FW_DIR            = firmware
DOC_DIR           = doc
RES_DIR           = res

#### 
# The files we either expect to be, or already are within
# the structure we just defined in the lines above.
##
# If the * is used as a wildcard, it has to be defined as a
# wildcard-operator.
SOURCES           = $(wildcard $(SOURCES_DIR)/*.c)
HEADERS           = $(wildcard $(HEADERS_DIR)/*.h)
# Our object-files will be generated via the compile-target,
# and we expect that files to be in the obj-directory and named as 
# their corresponding src-files (only with a changed suffix).
# To be as precise as possible and not forget any source file or so,
# we apply a nested pattern-substitution of the SOURCES-variable
# here.
# The sources-directory in SOURCES is pattern-matched and replaced
# with the objects-directory, as well as the .c-extension of every
# source-file's title is replaced with .o.
# Keep in mind that the inner pattern substitution
# ($(patsubst %.c,%.o,$(SOURCES)) is executed before the outer one. The
# result of the inner substitution is handled as a new make-variable
# within the course of the outer substitution.
OBJECTS           = $(patsubst $(SOURCES_DIR)/%,$(OBJECTS_DIR)/%,$(patsubst %.c,%.o,$(SOURCES)))

# ELF and firmware will be generated from the respective targets down below.
ELF_FILENAME      = compiled_and_linked.elf
FW_FILENAME       = firmware.hex
# FQFN: short for "Fully Qualified File-Name" (path + filename)
ELF_FQFN          = $(addprefix $(ELF_DIR)/, $(ELF_FILENAME))
FW_FQFN           = $(addprefix $(FW_DIR)/, $(FW_FILENAME))

####
# Tools and options
##

# COMPILER (used to build the object-files)
CC                = avr-gcc
# Compiler-options
CC_MACHINE_OPTION = -mmcu=atmega328p
CC_FLAGS          = -c -Os -D F_CPU=16E6 -I"$(HEADERS_DIR)"

# LINKER (used to link all the object files into one single
# (theoretically executable) ELF-file).
# NOTE: We use the compiler here, it invokes the linker!
LD                =$(CC)
# Linker-options
LD_MACHINE_OPTION = $(CC_MACHINE_OPTION)
LD_FLAGS          = -o

# FIRMWARE-EXTRACTOR
FW_EXTRACTOR      = avr-objcopy
# Firmware-extractor-options
FW_FORMAT         = -O ihex

# FLASH-TOOL
FLA_TOOL          = avrdude
# Flash-tool-options
FLA_PARTNO        = -p atmega328p
FLA_PROGRAMMER    = -c arduino
FLA_PORT          = -P /dev/ttyACM0
FLA_BAUDRATE      = -b 115200
FLA_MEMTYPE       = flash
FLA_OP            = w
FLA_FORMAT        = i
FLA_FLASHSTRING   = -U $(FLA_MEMTYPE):$(FLA_OP):$(FW_FQFN):$(FLA_FORMAT)
FLA_PARAMS        = $(FLA_PARTNO) $(FLA_PROGRAMMER) $(FLA_PORT) \
                    $(FLA_BAUDRATE) $(FLA_FLASHSTRING)

# DOC-TOOL
DOC_TOOL          = doxygen
DOC_OPTIONS       = $(RES_DIR)/doxyfile
DOC_WATCH         = /html/index.html

####
# TARGETS
##
.PHONY: clean

#all: compile link firmware flash doc
all: compile link firmware 

# Compile sources to object-files.
compile: $(OBJECTS)

$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJECTS_DIR)
	@echo Compiling $< to $@!
	@$(CC) $(CC_MACHINE_OPTION) $(CC_FLAGS) $< -o $@

# Link the object files, a ELF-file will be produced.
$(ELF_FQFN): $(OBJECTS)
	@mkdir -p $(ELF_DIR)
	@echo Linking object-files to $(ELF_FQFN)!
	@$(LD) $(LD_MACHINE_OPTION) $^ -o $(ELF_FQFN)

link: $(ELF_FQFN)

elf: link

# Extract the firmware from elf to HEX-file.
firmware: $(FW_FQFN)

$(FW_FQFN): elf
	@echo Extracting firmware via $(FW_EXTRACTOR) to $(FW_FQFN)!
	@mkdir -p $(FW_DIR)
	@$(FW_EXTRACTOR) $(FW_FORMAT) $(ELF_FQFN) $(FW_FQFN)

# Flashes the firmware.

flash: firmware
	@echo Flashing via port $(patsubst -P,,$(FLA_PORT))!
	@$(FLA_TOOL) $(FLA_PARAMS)

# Generates the doxygen-documentation, takes config from a "doxyfile"
# DOC_WATCH is a file we monitor to check whether a doc-build is
# necessary if preqrequisites are unchanged.
doc: $(DOC_DIR)$(DOC_WATCH)

$(DOC_DIR)$(DOC_WATCH): $(SOURCES) $(HEADERS) $(DOC_OPTIONS)
	@echo Generating documentation in $(DOC_DIR)!
	@mkdir -p $(DOC_DIR)
	@$(DOC_TOOL) $(DOC_OPTIONS)

# Clears every artifact generated from the sources.
clean:
	@echo Removing build-artifacts...
	@rm -r -f $(OBJECTS_DIR)
	@rm -r -f $(ELF_DIR)
	@rm -r -f $(FW_DIR)
	@rm -r -f $(DOC_DIR)
