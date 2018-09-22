#include "opengl.h"

const double INF = (double) 1e9;
#define N 13000 
int id,n;

struct Point 
{ 
    int x,y;
    double slope_order;
    Point() {} 
    Point(int a,int b) 
    { 
        x = a;
        y = b; 
    } 
    // draw this point in 2d space 
    void draw() { } 
    // draw a line b/w this two points 
    void drawTo(Point that) { } 
    // returns string representation of that point 
    string toString() 
    { 
        string pointx = "", pointy = ""; 
        int p = x; 
        while(p) 
        { 
            int c = p % 10; 
            pointx.push_back(char(c+'0'));
            p /= 10; 
        }
        p = y; 
        while(p) 
        { 
            int c = p % 10; 
            pointy.push_back(char(c+'0')); 
            p /= 10; 
        } 
        reverse(pointx.begin(),pointx.end());
        reverse(pointy.begin(),pointy.end()); 
        return "(" + pointx + "," + pointy + ")";
    } 
    // is lexicographically smaller 
    int compareTo(Point that) 
    { 
        if(y == that.y) return x < that.x; 
        return y < that.y; 
    } 
    // slope b/w 2 points 
    double slopeTo(Point that) 
    { 
        if(that.x == x && that.y == y) return -INF;
        if(that.x == x) return INF; 
        if(that.y == y) return 0.0;
        double slope = (double)(that.y - y) / (double)(that.x - x); 
        return slope; 
    }

    bool operator==(const Point& that) const
    {
        return x == that.x && y == that.y;
    }

};

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator<(const Point& a, const Point &b)
{
    if(a.y == b.y) return a.x < b.x;
    return a.y < b.y;
}

struct vertex{
    GLfloat x,y;
};

Point points[N];
vector<Point>collinear[N];
vertex *vtcs,*vt;
int mx,t;

void clr()
{
    for(int i=0;i<N;++i)
    {
        collinear[i].clear();
    }
}

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glBegin(GL_POINTS);
        glColor3f(0.4f, 0.7f, 0.3f);
        glVertex2f(0.289f,0.192f);
    glEnd();

    glVertexPointer(2,GL_FLOAT,sizeof(vertex),vtcs);
    glDrawArrays(GL_POINTS,0,n);

    for(int i=0;i<id;++i)
    {
        vertex *vtls = new vertex[2];
        vtls[0].x = (GLfloat)collinear[i][0].x / (mx*2);
        vtls[0].y = (GLfloat)collinear[i][0].y / (mx*2);

        vtls[1].x = (GLfloat)collinear[i][(int)collinear[i].size()-1].x / (mx*2);
        vtls[1].y = (GLfloat)collinear[i][(int)collinear[i].size()-1].y / (mx*2);
        //cerr<<vtls[0].x<<" "<<vtls[0].y<< " "<<vtls[1].x<<" "<<vtls[1].y<<endl;

        glColor3f(0.1f, 1.0f, 0.4f);
        glVertexPointer(2,GL_FLOAT,sizeof(vertex),vtls);
        glDrawArrays(GL_LINES,0,2);
    }

    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(-1.0f,0.0f);
        glVertex2f(1.0f,0.0f);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(0.0f,-1.0f);
        glVertex2f(0.0f,1.0f);
    glEnd();
 
    glFlush();  // Render now
}

void init()
{
    glutCreateWindow("Pattern Lab 3");
    glutInitWindowSize(800,600);
    glutInitWindowPosition(50,50);
    glPointSize(8);
    glLineWidth(3);
    glEnableClientState(GL_VERTEX_ARRAY);
    glutDisplayFunc(display);
    glutMainLoop();
    return;
}

void brute()
{
    clr();
    id = 0;
    for(int i=0;i<n-3;++i)
    {
        for(int j=i+1;j<n-2;++j)
        {
            for(int k=j+1;k<n-1;++k)
            {
                for(int l=k+1;l<n;++l)
                {
                    //cerr<<points[i].slopeTo(points[j]) << " " << points[i].slopeTo(points[k]) << " " << points[i].slopeTo(points[l])<<endl;
                    if(points[i].slopeTo(points[j]) == points[i].slopeTo(points[k]) && points[i].slopeTo(points[j]) == points[i].slopeTo(points[l]))
                    {
                        //cerr<<"YES"<<endl;
                        collinear[id].push_back(points[i]); collinear[id].push_back(points[j]);
                        collinear[id].push_back(points[k]); collinear[id++].push_back(points[l]);
                    }
                }
            }
        }
    }
    vtcs = new vertex[n];
    for(int i=0;i<n;++i)
    {
        vtcs[i].x = (GLfloat)points[i].x / (mx*2);
        vtcs[i].y = (GLfloat)points[i].y / (mx*2);
    }
    //cerr<<"ID : "<<id<<endl;
    for(int i=0;i<id;++i)
    {
        sort(collinear[i].begin(),collinear[i].end());
        cout<<collinear[i][0].toString();
        for(int j=1;j<(int)collinear[i].size();++j)
        {
            cout <<" -> "<< collinear[i][j].toString();
            collinear[i][j-1].drawTo(collinear[i][j]);
        }
        cout<<endl;
    }
    init();
}

bool cmp(Point a, Point b)
{
    return a.slope_order < b.slope_order;
}



void fast() 
{
    if(n < 4) return;
    clr();
    id = 0;
    for(int i=0;i<n;++i)
    {
        vector<Point>slopeorder;
        for(int j=0;j<n;++j)
        {
            if(i == j) continue;
            Point slope_point = Point(points[j].x,points[j].y);
            slope_point.slope_order = points[i].slopeTo(points[j]);
            slopeorder.push_back(slope_point);
            //cerr<<slope_point.slope_order<<" ";
        }
        //cerr<<endl;
        sort(slopeorder.begin(),slopeorder.end(),cmp);
        int cnt = 0;
        double basic = slopeorder[0].slope_order;
        vector<Point>buffer;
        for(int j=0;j<(int)slopeorder.size();++j)
        {
            if(slopeorder[j].slope_order == basic)
            {
                buffer.push_back(slopeorder[j]);
                cnt++;
            }
            else{
                if(cnt >= 3)
                {
                    collinear[id].push_back(points[i]);
                    for(int k=0;k<(int)buffer.size();++k)
                    {
                        collinear[id].push_back(buffer[k]);
                    }
                    id++;
                }
                buffer.clear();
                basic = slopeorder[j].slope_order;
                cnt = 1;
                buffer.push_back(slopeorder[j]);
            }
        }
        if(cnt >= 3)
        {
            collinear[id].push_back(points[i]);
            for(int k=0;k<(int)buffer.size();++k)
            {
                collinear[id].push_back(buffer[k]);
            }
            id++;
        }
    }

    
    vtcs = new vertex[n];
    for(int i=0;i<n;++i)
    {
        vtcs[i].x = (GLfloat)points[i].x / (mx*2);
        vtcs[i].y = (GLfloat)points[i].y / (mx*2);
    }
    //cerr<<"ID : "<<id<<endl;

    vector< vector<Point> > munique;

    // handle duplicates

    for(int i=0;i<id;++i)
    {
        sort(collinear[i].begin(),collinear[i].end());
        cout<<collinear[i][0].toString();
        for(int j=1;j<(int)collinear[i].size();++j)
        {
            cout <<" -> "<< collinear[i][j].toString();
            collinear[i][j-1].drawTo(collinear[i][j]);
        }
        cout<<endl;
    }
    init();
} 



int main(int argc, char** argv) 
{
    glutInit(&argc,argv);
    

    if(argc == 2) { 
        ifstream in(argv[1]); 
        in>>n; 
        for(int i=0;i<n;++i)
        { 
            in>>points[i].x>>points[i].y; 
            //cout<<points[i].toString()<<endl;
            mx= max(mx,points[i].x);
            mx = max(mx, points[i].y );
        }
        fast();

        cin.get();
    } else
    { 
        cin>>n; 
        for(int i=0;i<n;++i) 
        { 
            cin>>points[i].x>>points[i].y; 
            //cout<<points[i].toString()<<endl; 
        }
        brute();
        cin.get(); 
        cin.get(); 
    } 
    return 0; 
}
