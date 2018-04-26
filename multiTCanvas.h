#include "TCanvas.h"
#include "TPad.h"
#include <vector>
#include <exception>
#include "OutofBoundsException.h"

class multiTCanvas
{
  public:
    multiTCanvas(const char* name, const char* title, int ww, int wh, unsigned int npanel);
    ~multiTCanvas();

    void SetName(const char* n);
    void SetTitle(const char* t);
    void SetLeftMargin(float m);
    void SetRightMargin(float m);
    void SetTopMargin(float m);
    void SetBottomMargin(float m);
    TCanvas* GetCanvas();
    TPad* GetPad(int i);

    void Draw();
    template<class T>
    void DrawAll(queue<T*>);
    void cd(int i);
    float GetScaleFactor(int i);

  private:
    TCanvas* c;
    std::vector<TPad*> pads;
    float margins[4]; // as percentage of total canvas
    unsigned int npanel;
    std::vector<float> posts;
    bool initialized = false;
};

multiTCanvas::multiTCanvas(const char* name, const char* title, int ww, int wh, unsigned int n)
  : npanel(n)
{
  c = new TCanvas(name, title, ww, wh);

  margins[0] = 0.05; // left
  margins[1] = 0.006; // right
  margins[2] = 0.03; // top
  margins[3] = 0.12; // bottom
}

multiTCanvas::~multiTCanvas()
{
  for(unsigned int i = 0; i < pads.size(); ++i)
    delete pads[i];
  delete c;
}

void multiTCanvas::SetName(const char* n)
{  c->SetName(n); }

void multiTCanvas::SetTitle(const char* t)
{  c->SetTitle(t); }

void multiTCanvas::SetLeftMargin(float m)
{  margins[0] = m; }

void multiTCanvas::SetRightMargin(float m)
{  margins[1] = m; }

void multiTCanvas::SetTopMargin(float m)
{  margins[2] = m; }

void multiTCanvas::SetBottomMargin(float m)
{ margins[3] = m; }

TCanvas* multiTCanvas::GetCanvas()
{ return c; }

TPad* multiTCanvas::GetPad(int i)
{ 
  if(i < 1 || i > npanel)
  {
    cout << "multiTCanvas::GetPad Pad index out of range" << endl;
    return NULL;
  }
  return pads[i-1];
}

void multiTCanvas::Draw()
{
  initialized=true;
  posts.push_back(0);
  for(unsigned int i = 1; i < npanel; ++i)
    posts.push_back( ((float)(i)/(float)npanel)*(1-margins[0]-margins[1]) + margins[0] );
  posts.push_back(1);

  for(unsigned int i = 0; i < npanel; ++i)
  {
    pads.push_back(new TPad(Form("pad_%d",i),"",posts[i], 0, posts[i+1], 1));
    pads[i]->SetPad(posts[i], 0, posts[i+1], 1);
    pads[i]->SetLeftMargin(0);
    pads[i]->SetRightMargin(0);
    pads[i]->SetTopMargin(margins[2]);
    pads[i]->SetBottomMargin(margins[3]);
  }
  pads[0]->SetLeftMargin(margins[0]/posts[1]); // convert the margins to local pad coords
  pads[npanel-1]->SetRightMargin(margins[1]/(posts[npanel-1] - posts[npanel-2]));

  c->cd();
  for(unsigned int i = 0; i < npanel; ++i)
    pads[i]->Draw();
}

template<class T> // must be a TH1 
void multiTCanvas::DrawAll(queue<T*> in){
  if (in.size()>npanel)
  {
    cout<<"Too many entries at multiTCanvas  Name: "<<c->GetName()<<endl;
    queue<T*> temp;
    for (int i = 0; i < npanel; ++i)
    {
      temp.push(in.front());
      in.pop();
    }
    in=temp;
  }
  this->Draw();
  int iPad=1;
  while (!in.empty())
  {
    try{
      this->cd(iPad);
    }
    catch(OutofBoundsException<TCanvas>& e){
     // cout<<"here->"<<endl;
      cout<<e.what()<<", "<<e.getError()<<endl;
    }
    in.front()->DrawCopy();
    in.pop();
    iPad++;
  }

}
  
void multiTCanvas::cd(int i)
{
  if(i < 1 || i > npanel||!initialized)
  {
    string error;
    if (!initialized)
    {
      error = "unitialized canvas call Draw() before cd()"
    }
    else{
      error = string(c->GetName());
    }
    OutofBoundsException<TCanvas> myException(error,npanel,i);
    if (i<1)
    {
      pads[1]->cd();
    }
    else{
      pads[npanel]->cd();
    }
    throw myException;
  }
  else{
    pads[i-1]->cd();
  }
}

float multiTCanvas::GetScaleFactor(int i)
{
  //return 1./(npanel*( posts[i]-posts[i-1] - pads[i-1]->GetLeftMargin() - pads[i-1]->GetRightMargin() ));
  return 1 - pads[i-1]->GetLeftMargin() - pads[i-1]->GetRightMargin();
}
