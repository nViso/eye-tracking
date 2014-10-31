function [x y width height] = getMatlabWindowPosition() 

desktop = com.mathworks.mde.desk.MLDesktop.getInstance;
desktopMainFrame = desktop.getMainFrame;
% Get desktop dimensions
desktopDims = desktopMainFrame.getSize;
width = desktopDims.getWidth;
height = desktopDims.getHeight;

desktopLocation = desktopMainFrame.getLocation;
x = desktopLocation.getX;
y = desktopLocation.getY;