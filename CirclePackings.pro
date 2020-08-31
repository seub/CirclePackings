unix:QMAKE_CXXFLAGS_RELEASE -= -O
unix:QMAKE_CXXFLAGS_RELEASE -= -O1
unix:QMAKE_CXXFLAGS_RELEASE -= -O2
unix:QMAKE_CXXFLAGS_RELEASE *= -O3

QT += widgets

HEADERS += \
    point.hpp \
    curve.hpp \
    mobius.hpp \
    circle.hpp \
    vertex.hpp \
    graph.hpp \
    empty.hpp \
    graph_completer.hpp \
    graph_cyclic_sorter.hpp \
    graph_orienter.hpp \
    graph_tiler.hpp \
    algo.hpp \
    canvas.hpp \
    window.hpp \
    input_menu.hpp \
    output_menu.hpp \
    graph_colorer.hpp \
    basic_types.hpp \
    maths_contents.hpp \
    tests.hpp \
    positions.hpp \
    top_algo.hpp \
    canvas_delegate.hpp \
    canvas_delegate_draw_curve.hpp \
    canvas_delegate_draw_graph.hpp \
    canvas_delegate_show_cp.hpp \
    canvas_delegate_tests.hpp \
    canvas_delegate_show_live.hpp \
    graph_triangulater.hpp \
    triangle.hpp \
    triangulation.hpp \
    canvas_delegate_riemann.hpp \
    riemann_positions.hpp \
    triangulation_search_tree.hpp \
    positions_mixer.hpp \
    top_menu.hpp \
    configuration.hpp \
    graph_sampler.hpp \
    affine_transform.hpp \
    circle_packings_application.hpp \
    help_browser.hpp

SOURCES += \
    point.cpp \
    main.cpp \
    curve.cpp \
    mobius.cpp \
    circle.cpp \
    vertex.cpp \
    graph.cpp \
    empty.cpp \
    graph_completer.cpp \
    graph_cyclic_sorter.cpp \
    graph_orienter.cpp \
    graph_tiler.cpp \
    algo.cpp \
    canvas.cpp \
    window.cpp \
    input_menu.cpp \
    output_menu.cpp \
    graph_colorer.cpp \
    canvas_delegate.cpp \
    canvas_delegate_draw_graph.cpp \
    canvas_delegate_draw_curve.cpp \
    canvas_delegate_show_cp.cpp \
    maths_contents.cpp \
    tests.cpp \
    canvas_delegate_tests.cpp \
    positions.cpp \
    top_algo.cpp \
    canvas_delegate_show_live.cpp \
    graph_triangulater.cpp \
    triangle.cpp \
    triangulation.cpp \
    canvas_delegate_riemann.cpp \
    riemann_positions.cpp \
    triangulation_search_tree.cpp \
    positions_mixer.cpp \
    top_menu.cpp \
    basic_types.cpp \
    configuration.cpp \
    graph_sampler.cpp \
    affine_transform.cpp \
    circle_packings_application.cpp \
    help_browser.cpp

RESOURCES += \
    resources.qrc

mac:ICON = images/icon-osx.icns
win32:RC_FILE = CirclePackings.rc
