function tbxStruct=demos
%DEMOS Demo list for the OpenCV toolbox.

if nargout==0, demo toolbox; return; end

tbxStruct.Name='OpenCV';
tbxStruct.Type='toolbox';

tbxStruct.Help= {
    'OpenCV is powerful open source'
    'computer vision library'
    ' '
     };

tbxStruct.DemoList={
    'Canny Demo', 'cannydemo'
    'Contour Demo', 'contdemo'
    'Flood Fill Demo', 'filldemo'
    'Optical Flow Demo', 'flowdemo'
};
