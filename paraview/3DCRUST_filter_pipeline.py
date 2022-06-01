# trace generated using paraview version 5.9.0

#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# get active source.
tet_area_load_bedding_50xyvtp = GetActiveSource()

# get active view
renderView1 = GetActiveViewOrCreate('RenderView')

# get display properties
tet_area_load_bedding_50xyvtpDisplay = GetDisplayProperties(tet_area_load_bedding_50xyvtp, view=renderView1)

# set scalar coloring
ColorBy(tet_area_load_bedding_50xyvtpDisplay, ('POINTS', 'principalStress'))

# rescale color and/or opacity maps used to include current data range
tet_area_load_bedding_50xyvtpDisplay.RescaleTransferFunctionToDataRange(True, False)

# show color bar/color legend
tet_area_load_bedding_50xyvtpDisplay.SetScalarBarVisibility(renderView1, True)

# get color transfer function/color map for 'principalStress'
principalStressLUT = GetColorTransferFunction('principalStress')
principalStressLUT.RGBPoints = [-11.513099670410156, 0.231373, 0.298039, 0.752941, -5.17758983373642, 0.865003, 0.865003, 0.865003, 1.157920002937317, 0.705882, 0.0156863, 0.14902]
principalStressLUT.ScalarRangeInitialized = 1.0

# get opacity transfer function/opacity map for 'principalStress'
principalStressPWF = GetOpacityTransferFunction('principalStress')
principalStressPWF.Points = [-11.513099670410156, 0.0, 0.5, 0.0, 1.157920002937317, 1.0, 0.5, 0.0]
principalStressPWF.ScalarRangeInitialized = 1

ImportPresets(filename='C:\Users\juanc\Documents\TUM\3. Winter Semester 2021\SL_Lab\SL_Preset_01_Hot_Cold.json')

# Apply a preset using its name. Note this may not work as expected when presets have duplicate names.
principalStressLUT.ApplyPreset('Preset_softwarelab', True)

# Apply a preset using its name. Note this may not work as expected when presets have duplicate names.
principalStressPWF.ApplyPreset('Preset_softwarelab', True)

# create a new 'Tube'
tube1 = Tube(registrationName='Tube1', Input=tet_area_load_bedding_50xyvtp)
tube1.Scalars = ['POINTS', 'principalStress']
tube1.Vectors = [None, '1']
tube1.Radius = 0.0100956

# Properties modified on tube1
tube1.Vectors = ['POINTS', '1']
tube1.Radius = 0.005
tube1.VaryRadius = 'By Scalar'
tube1.RadiusFactor = 1.0

# show data in view
tube1Display = Show(tube1, renderView1, 'GeometryRepresentation')

# trace defaults for the display properties.
tube1Display.Representation = 'Surface'
tube1Display.ColorArrayName = ['POINTS', 'principalStress']
tube1Display.LookupTable = principalStressLUT
tube1Display.SelectTCoordArray = 'None'
tube1Display.SelectNormalArray = 'TubeNormals'
tube1Display.SelectTangentArray = 'None'
tube1Display.OSPRayScaleArray = 'TubeNormals'
tube1Display.OSPRayScaleFunction = 'PiecewiseFunction'
tube1Display.SelectOrientationVectors = 'None'
tube1Display.ScaleFactor = 0.10183680888405044
tube1Display.SelectScaleArray = 'None'
tube1Display.GlyphType = 'Arrow'
tube1Display.GlyphTableIndexArray = 'None'
tube1Display.GaussianRadius = 0.005091840444202522
tube1Display.SetScaleArray = ['POINTS', 'TubeNormals']
tube1Display.ScaleTransferFunction = 'PiecewiseFunction'
tube1Display.OpacityArray = ['POINTS', 'TubeNormals']
tube1Display.OpacityTransferFunction = 'PiecewiseFunction'
tube1Display.DataAxesGrid = 'GridAxesRepresentation'
tube1Display.PolarAxes = 'PolarAxesRepresentation'

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
tube1Display.ScaleTransferFunction.Points = [-1.0, 0.0, 0.5, 0.0, 1.0, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
tube1Display.OpacityTransferFunction.Points = [-1.0, 0.0, 0.5, 0.0, 1.0, 1.0, 0.5, 0.0]

# hide data in view
Hide(tet_area_load_bedding_50xyvtp, renderView1)

# show color bar/color legend
tube1Display.SetScalarBarVisibility(renderView1, True)

# find source
tet_area_load_beddingvtu = FindSource('tet_area_load_bedding.vtu')

# update the view to ensure updated data information
renderView1.Update()

# create a new 'Threshold'
threshold1 = Threshold(registrationName='Threshold1', Input=tube1)
threshold1.Scalars = ['POINTS', 'principalStress']
threshold1.ThresholdRange = [-11.513099670410156, 1.157920002937317]

# show data in view
threshold1Display = Show(threshold1, renderView1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
threshold1Display.Representation = 'Surface'
threshold1Display.ColorArrayName = ['POINTS', 'principalStress']
threshold1Display.LookupTable = principalStressLUT
threshold1Display.SelectTCoordArray = 'None'
threshold1Display.SelectNormalArray = 'TubeNormals'
threshold1Display.SelectTangentArray = 'None'
threshold1Display.OSPRayScaleArray = 'TubeNormals'
threshold1Display.OSPRayScaleFunction = 'PiecewiseFunction'
threshold1Display.SelectOrientationVectors = 'None'
threshold1Display.ScaleFactor = 0.10183680888405044
threshold1Display.SelectScaleArray = 'None'
threshold1Display.GlyphType = 'Arrow'
threshold1Display.GlyphTableIndexArray = 'None'
threshold1Display.GaussianRadius = 0.005091840444202522
threshold1Display.SetScaleArray = ['POINTS', 'TubeNormals']
threshold1Display.ScaleTransferFunction = 'PiecewiseFunction'
threshold1Display.OpacityArray = ['POINTS', 'TubeNormals']
threshold1Display.OpacityTransferFunction = 'PiecewiseFunction'
threshold1Display.DataAxesGrid = 'GridAxesRepresentation'
threshold1Display.PolarAxes = 'PolarAxesRepresentation'
threshold1Display.ScalarOpacityFunction = principalStressPWF
threshold1Display.ScalarOpacityUnitDistance = 0.051753767034333834
threshold1Display.OpacityArrayName = ['POINTS', 'TubeNormals']

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
threshold1Display.ScaleTransferFunction.Points = [-1.0, 0.0, 0.5, 0.0, 1.0, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
threshold1Display.OpacityTransferFunction.Points = [-1.0, 0.0, 0.5, 0.0, 1.0, 1.0, 0.5, 0.0]

# hide data in view
Hide(tube1, renderView1)

# show color bar/color legend
threshold1Display.SetScalarBarVisibility(renderView1, True)

# update the view to ensure updated data information
renderView1.Update()

#================================================================
# addendum: following script captures some of the application
# state to faithfully reproduce the visualization during playback
#================================================================

# get layout
layout1 = GetLayout()

#--------------------------------
# saving layout sizes for layouts

# layout/tab size in pixels
layout1.SetSize(1121, 755)

#-----------------------------------
# saving camera placements for views

# current camera placement for renderView1
renderView1.CameraPosition = [3.0251941631645423, 2.448537184942984, -0.5187160549172117]
renderView1.CameraFocalPoint = [0.5177313081399711, 0.5438707196084187, 0.6131090681510707]
renderView1.CameraViewUp = [-0.24300461331978482, -0.23877758028374904, -0.9401776561156618]
renderView1.CameraParallelScale = 0.8660254037844386

#--------------------------------------------
# uncomment the following to render all views
# RenderAllViews()
# alternatively, if you want to write images, you can use SaveScreenshot(...).