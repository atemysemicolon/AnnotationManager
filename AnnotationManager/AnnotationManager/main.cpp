#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <boost/algorithm/string.hpp>



using namespace std;

class ClassData
{


public:
    ClassData()
    {
        build_default_map();
    }
   std::vector<cv::Vec3b> label_colours;
   std::vector<std::string> label_names;
   cv::Mat_<uchar> labels_indices_image;
   cv::Mat labels_colours_image;
   cv::Mat labels_colours_image_translated;
   std::vector<int> map_indices;

    void build_default_map()
    {
        map_indices.clear();
        for(int i=0;i<std::min(this->label_names.size(), this->label_colours.size());i++)
            map_indices.push_back(i);
    }

   int find_label_index(cv::Vec3b colour)
   {
       int index=-1;
       for(std::vector<cv::Vec3b>::iterator it = label_colours.begin(); it != label_colours.end(); ++it)
       {
           if(*it == colour)
           {
                index = std::distance(label_colours.begin(), it);
                break;
           }
       }

       if(index>=0) return index;
       else return -1;
   }
   int find_label_index(std::string label_name)
   {
       int index=-1;
       for(std::vector<std::string>::iterator it = label_names.begin(); it != label_names.end(); ++it)
       {
           if(label_name.compare(*it)==0)
           {
                index = std::distance(label_names.begin(), it);
                break;
           }
       }

       if(index>=0) return index;
       else return -1;

   }

   std::string find_label_name(int label_index)
   {
       return label_names[label_index];

   }

   std::string find_label_name(cv::Vec3b colour)
   {
       int index=-1;
       for(std::vector<cv::Vec3b>::iterator it = label_colours.begin(); it != label_colours.end(); ++it)
       {
           if(*it == colour)
           {
                index = std::distance(label_colours.begin(), it);
                break;
           }
       }

       if(index>=0) return label_names[index];
       else return "null";
   }
   cv::Vec3b find_label_colour (int label_index)
   {
       return label_colours[label_index];

   }
   cv::Vec3b find_label_colour (std::string label_name)
   {
       int index=-1;
       for(std::vector<std::string>::iterator it = label_names.begin(); it != label_names.end(); ++it)
       {
           if(label_name.compare(*it)==0)
           {
                index = std::distance(label_names.begin(), it);
                break;
           }
       }

       if(index>=0) return label_colours[index];
       else return cv::Vec3b(1,1,1);
   }

   cv::Mat loadImagetoIndices(cv::Mat &label_image)
   {
       cv::Mat_<cv::Vec3b> &templatedImage = (cv::Mat_<cv::Vec3b> &)label_image;
       cv::Mat_<uchar> indexImage(label_image.rows, label_image.cols);
       for(int x=0;x<templatedImage.cols;x++)
           for(int y=0;y<templatedImage.rows;y++)
           {
               uchar index = find_label_index(templatedImage(y,x));

               if(index >=0 )
                   indexImage(y,x) = map_indices[index];

           }

       this->labels_indices_image = indexImage.clone();
       return indexImage;

   }

   cv::Mat loadIndicestoImage(cv::Mat &index_image)
   {
       cv::Mat_<uchar> templatedImage = (cv::Mat_<uchar> &) index_image;
       cv::Mat_<cv::Vec3b> colorImage(templatedImage.rows, templatedImage.cols);
       for(int x=0;x<templatedImage.cols;x++)
           for(int y=0;y<templatedImage.rows;y++)
           {
               uchar index = templatedImage(y,x);

               if( (index >=0) && (index <this->label_colours.size()) )
                   colorImage(y,x) = label_colours[index];

           }

       this->labels_colours_image_translated = colorImage.clone();
       return colorImage;


   }

   void loadLabelMat(cv::Mat &input_labels_colours)
   {
        this->labels_colours_image = input_labels_colours.clone();
   }

};


class CamVidData : public ClassData
{
public:
    CamVidData() : ClassData()
    {
        label_colours = {cv::Vec3b(0,0,0), cv::Vec3b(255,255,255), cv::Vec3b(0,0,255)};
        label_names = {"Label1", "Label2", "Label3"};
        build_default_map();
    }
};




int main()
{
    cout << "Hello World!" << endl;
    CamVidData newObj;
    cv::Mat img = cv::imread("test.png");

    cv::Mat indexIm = newObj.loadImagetoIndices(img);
    newObj.loadIndicestoImage(indexIm);



    cv::imshow("bla",newObj.labels_colours_image_translated);


    cv::waitKey(0);

    return 0;
}

