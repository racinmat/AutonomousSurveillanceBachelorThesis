cmake_minimum_required(VERSION 3.6)
project(SwarmDeployment)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(ROOT_SOURCE_DIR .)
include_directories("${ROOT_SOURCE_DIR}")
include_directories(GeneratedFiles)


set(CMAKE_AUTOMOC ON)

set(CMAKE_PREFIX_PATH "D:\\Qt\\5.8\\mingw53_32\\lib\\cmake")

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

get_filename_component(MINGW_DLL_DIR ${CMAKE_CXX_COMPILER} PATH)
install(FILES
        ${MINGW_DLL_DIR}/libgcc_s_dw2-1.dll
        ${MINGW_DLL_DIR}/libstdc++-6.dll
        ${MINGW_DLL_DIR}/libwinpthread-1.dll
        DESTINATION .
        )

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)


set(SOURCE_FILES

        Dubins/geom/shape/Circle.cpp
        Dubins/geom/shape/Circle.h
        Dubins/geom/shape/Ellipse.cpp
        Dubins/geom/shape/Ellipse.h
        Dubins/geom/shape/Polygon.cpp
        Dubins/geom/shape/Polygon.h
        Dubins/geom/shape/Shape.cpp
        Dubins/geom/shape/Shape.h
        Dubins/geom/shape/ShapePoint.cpp
        Dubins/geom/shape/ShapePoint.h
        Dubins/geom/Arc.cpp
        Dubins/geom/Arc.h
        Dubins/geom/Dubins.cpp
        Dubins/geom/Dubins.h
        Dubins/geom/geom.h
        Dubins/geom/Intersection.h
        Dubins/geom/Line.cpp
        Dubins/geom/Line.h
        Dubins/geom/MyMath.h
        Dubins/geom/Path.cpp
        Dubins/geom/Path.h
        Dubins/geom/Point.h
        Dubins/geom/Position.cpp
        Dubins/geom/Position.h
        Dubins/geom/Vector.h
        Dubins/dubins_test.cpp
        VCollide/AABB.H
        VCollide/build.cpp
        VCollide/coldetect.cpp
        VCollide/ColDetect.h
        VCollide/collide.cpp
        VCollide/EndPoint.H
        VCollide/matvec.H
        VCollide/moments.H
        VCollide/NBody.cpp
        VCollide/NBody.H
        VCollide/obb.H
        VCollide/overlap.cpp
        VCollide/overlap.H
        VCollide/PairData.cpp
        VCollide/PairData.H
        VCollide/Point2D.cpp
        VCollide/Point2D.h
        VCollide/Point3D.cpp
        VCollide/Point3D.h
        VCollide/RAPID.cpp
        VCollide/RAPID.H
        VCollide/RAPID_private.H
        VCollide/RAPID_version.H
        VCollide/Rectangle2D.cpp
        VCollide/Rectangle2D.h
        VCollide/sample_client.cpp
        VCollide/simple.cpp
        VCollide/simple2.cpp
        VCollide/Triangle3D.cpp
        VCollide/Triangle3D.h
        VCollide/VCollide.cpp
        VCollide/VCollide.H
        VCollide/VInternal.cpp
        VCollide/VInternal.H
        AoICoverageResolver.cpp
        AoICoverageResolver.h
        AStar.cpp
        AStar.h
        CarLikeAnalyticMotionModel.cpp
        CarLikeAnalyticMotionModel.h
        CarLikeControl.cpp
        CarLikeControl.h
        ClosedSet.cpp
        ClosedSet.h
        CollisionDetector.cpp
        CollisionDetector.h
        Configuration.cpp
        Configuration.h
        Core.cpp
        Core.h
        DistanceResolver.cpp
        DistanceResolver.h
        dubins_test.h
        EmptyPath.cpp
        EmptyPath.h
        Enums.h
        Goal.cpp
        Goal.h
        GoalGroup.cpp
        GoalGroup.h
        GoalInterface.cpp
        GoalInterface.h
        GuidingPathFactory.cpp
        GuidingPathFactory.h
        GuidingPathsCurrentPositions.cpp
        GuidingPathsCurrentPositions.h
        InputGenerator.cpp
        InputGenerator.h
        LinkedState.cpp
        LinkedState.h
        LoggerInterface.cpp
        LoggerInterface.h
        main.cpp
        Map.cpp
        Map.h
        MapFactory.cpp
        MapFactory.h
        MapGraph.cpp
        MapGraph.h
        MapProcessor.cpp
        MapProcessor.h
        MotionModel.cpp
        MotionModel.h
        Node.cpp
        Node.h
        NodeSet.cpp
        NodeSet.h
        NodeWrapper.cpp
        NodeWrapper.h
        NTupletGenerator.cpp
        NTupletGenerator.h
        Obstacle.cpp
        Obstacle.h
        OpenedSet.cpp
        OpenedSet.h
        Output.cpp
        Output.h
        Path.cpp
        Path.h
        PathFindingAlgorithm.h
        PathHandler.cpp
        PathHandler.h
        PathOptimizer.cpp
        PathOptimizer.h
        Persister.cpp
        Persister.h
        Point.cpp
        Point.h
        PointParticle.cpp
        PointParticle.h
        Random.cpp
        Random.h
        Rectangle.cpp
        Rectangle.h
        Resampler.cpp
        Resampler.h
        State.cpp
        State.h
        StateFactory.cpp
        StateFactory.h
        StateInterface.cpp
        StateInterface.h
        Strings.cpp
        Strings.h
        Uav.cpp
        Uav.h
        UavForRRT.cpp
        UavForRRT.h
        UavGroup.cpp
        UavGroup.h
        UavInterface.cpp
        UavInterface.h

        Gui.h
        Gui.cpp
        GuiDrawer.h
        GuiDrawer.cpp
        mainwindow.h
        mainwindow.cpp
        )

add_executable(SwarmDeployment ${SOURCE_FILES})

target_link_libraries(SwarmDeployment Qt5::Widgets Qt5::Core Qt5::Gui)

qt5_use_modules(SwarmDeployment Core Widgets Gui)

