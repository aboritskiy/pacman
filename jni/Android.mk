LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := NativeRenderer
LOCAL_CFLAGS    := -Wno-psabi
LOCAL_SRC_FILES := NativeRenderer.cpp\
                    GLESUtils.cpp\
                    GameRenderer.cpp\
                    data/FloatPosition.cpp\
                    data/GameState.cpp\
                    data/IntPosition.cpp\
                    data/MotionDirection.cpp\
                    model/GameModel.cpp\
                    model/PersonModel.cpp\
                    model/PacManModel.cpp\
                    model/GhostModel.cpp\
                    model/PinkyModel.cpp\
                    model/InkeyModel.cpp\
                    model/ClydeModel.cpp\
                    view/EnergizerView.cpp\
                    view/FieldView.cpp\
                    view/GameView.cpp\
                    view/GhostView.cpp\
                    view/TabletsView.cpp\
                    view/PacManView.cpp\
                    view/ViewConstants.cpp\
                    view/ScoreView.cpp\
                    view/LivesView.cpp\
                    view/GameStateView.cpp\
                    view/text/TextureRegion.cpp\
                    view/text/TextString.cpp\
                    view/text/StringDrawer.cpp

LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)