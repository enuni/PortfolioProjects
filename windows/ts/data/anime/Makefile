
WIN_BIN			=	$(WINDOWS_ROOT)/bin

ANIMATION_SRC		=	./src
ANIMATION_DST		=	./dst

IMAGE_DATA		=	./../src/image_data
SOUND_DATA		=	./../sound

do-build : $(ANIMATION_DST)/*.aaf

$(ANIMATION_DST)/*.aaf $(ANIMATION_DST)/*.h $(ANIMATION_DST)/*.inc : $(ANIMATION_SRC)/*.csf
	ruby $(WIN_BIN)/AnimationConverter.rb \
-ob $(IMAGE_DATA)/obj_main_3d_data.h \
-bat $(ANIMATION_SRC)/anime_data.txt \
-o $(ANIMATION_DST)/anime_data \
-c
#-se $(SOUND_DATA)/sound_data.sadl \

clean:
	rm -f $(ANIMATION_DST)/*.aaf $(ANIMATION_DST)/*.h $(ANIMATION_DST)/*.inc

