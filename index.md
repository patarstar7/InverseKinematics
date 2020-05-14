[![Video of Project](https://fbcdn-sphotos-g-a.akamaihd.net/hphotos-ak-prn2/n1/1920134_10152614278373356_2084939469_n.jpg)](http://www.youtube.com/watch?v=rfVsfdw7alE)

For our final project, we created a program that uses inverse kinematics to animate a three-dimensional kinematic arm tracking a moving target. The IK technique can be toggled between using the Jacobian transpose and cyclic coordinate descent. In order to facilitate diverse and interesting configurations, arms can incorporate both ball joints and prism joints; additionally, the system supports branching arms and operating multiple arms at once. We have included a few preset tracks for the targets to follow, which animate the arms in circular, figure-eight, and spiral motions to demonstrate the performance of our IK implementation. Our program has an interactive aspect, as it allows for manual control of the target as well.


In order to implement our IK arm, we decided to represent the arm itself as a tree-like structure. Each segment has an individual IK method that will compute that segment’s piece of the IK process. That way, a call to the root segment’s IK method will travel recursively up through all of the arm’s segments, and the arm will then be moved accordingly.

> ![](https://scontent-b.xx.fbcdn.net/hphotos-ash3/t1/1891049_10152614277938356_1925092304_n.jpg)   ![](https://scontent-b.xx.fbcdn.net/hphotos-prn2/t1/1782055_10152614277948356_404400327_n.jpg)    ![](https://fbcdn-sphotos-d-a.akamaihd.net/hphotos-ak-ash3/t1/1780721_10152614277928356_1118461624_n.jpg)

> _Comparing CCD and Jacobian transpose. On the left is the original configuration, with the red target. The CCD solution is in the middle, and the transpose solution is on the right. The configuration on the right looks less angular than the middle, showing how it better incorporates the motion of all joints than the CCD solution._


For cyclic coordinate descent, the IK solution is simple. Starting with the furthest-out segments of the arm, the segments are moved to minimize the distance from the end effector to the goal. This is done all the way down to the root segment, and then the process repeats from the beginning. Animating the arm this way results in violent, jerking movements, although by speeding up the rate at which our IK calculations are performed (doing more IK iterations per unit time), the movement is smoothed out somewhat. CCD generally reaches the target faster than the Jacobian transpose method, but is unnatural and often causes the arm to bend in ways that are inefficient.


Using the Jacobian transpose is a much more graceful method. The basic equation we use is this: Δθ = JT * e * s. Δθ is the resulting change in angle (or in the case of prism joints, length), JT is the transpose of the Jacobian, e is the vector from the end effector to the target, and s is a weight. Because we are using the Jacobian transpose, Δθ = JT * e * s can be calculated independently for each joint, where the Jacobian is filled with values only for that joint. Therefore, in our solution, each segment computes the Δθ for its attached joint independently. The scaling is accomplished by a flat weight for most values of Δθ, but as the end effector approaches the target, the weight is changed to an appropriately small value such that the end effector doesn’t engage in jumpy movement around the target. An additional perk of the Jacobian transpose method is that cases in which the target is out of reach of the arm are handled correctly, as the arm will straighten to its maximum length directly towards the target.


To facilitate branching arms, the value of Δθ must be calculated in order to minimize the distance between all end effectors and their associated targets. We accomplish this by calculating Δθ for a joint as if it were in a non-branching arm for each end effector, then averaging out the results. For example, a segment S branches to another segment with end effector E1, as well as a second segment with end effector E2. We then calculate Δθ with the above methods for S’s joint as it applies to E1. Then we do the calculation for S’s joint for it to move E2. Taking the mean of these two values of Δθ, we reach a half-way point that will minimize the distance to both, without favoring one end effector over another.

 
> ![](https://scontent-b.xx.fbcdn.net/hphotos-prn1/t1/1966772_10152614277933356_1653263791_n.jpg)

> _Even though neither red target is in reach, the arm configures itself in such a way that the distances to both targets are equally minimized._


In order to keep the math relatively simple, we kept all coordinates, directions, and angles in the world coordinate system. This way, there is no hassle of converting between a local coordinate system for each joint with the world coordinates. Angles are represented by the Euler parameterization, based on the Cartesian axes used by OpenGL. Therefore rotating a joint is accomplished by combining a series of axis-based rotation matrices (R = Rz * Ry * Rx). One potential downside of this implementation is that the system is susceptible to gimbal lock, although we have not noticed this as an issue. We made the observation that in cases where the arm is in a straightened-out configuration and the target is right in the middle of the arm, it bends at a very slow rate to reach the target. This is because the values filling the Jacobian (which are dpi/dθj) reach very miniscule quantities, similar to the case in which the target is out of reach of the outstretched arm. It is a frustrating situation that occasionally leads to some inaccurate target tracking.

> ![](https://scontent-a.xx.fbcdn.net/hphotos-ash3/t1/1623671_10152614277963356_543283826_n.jpg)

> _Because any movement of the joints has a diminutive effect on minimizing the distance from the end effector to the target, the IK function returns miniscule values of Δθ._

Prism joints (represented in our program as orange segments, in contrast to the green segments that denote ball joints) were a simple addition to our IK system. They differ from ball joints in that they have only one degree of freedom, rather than three, so they have fewer Jacobian entries. Additionally, our prism joints are constrained by a minimum and maximum length. This is done simply by checking that the change in length calculated by the IK function against the minimum and maximum values, and only moving the length between those constraints.

> ![](https://fbcdn-sphotos-c-a.akamaihd.net/hphotos-ak-frc1/t1/1891061_10152614277958356_829948289_n.jpg) 

> _An example of two prism joints  of varying length (colored orange)_
***

**Using the Program**

The program takes two command line parameters to set up the scene using some preset configurations. The first number determines the arm that is used, and the second sets the automated target paths. There are 5 arm presets and 3 target presets, varying in complexity. As an example, running the program with the line “./ik 3 2” will create a simple branching arm, where one end effector follows a circular path and the other follows a figure-eight. 

Here are the commands to use while running the code:
* P – Pause/play the arm movement
* M – Toggle between CCD and transpose modes (set to transpose by default)
* T – Stop automatic target paths
* W, A, S, D, (Q, E) – Manually move the target around
* +/-  - Zoom in an out
* Mouse – Click and drag to control view

