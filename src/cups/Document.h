#pragma once

#include <string>
#include <experimental/string_view>

namespace cups
{
    enum E_DOCUMENT_TYPE
    {
        EDT_TEXT,
        EDT_IMAGE,
        EDT_PDF,
        EDT_POSTSCRIPT,
        EDT_UNKNOW,
        EDT_COUNT
    };
    
    class Document
    {
        public:
            Document(const std::string &&filename)
                : filename(filename), totalPages(1)
            {
            }
        
            int getTotalPages() const
            {
                return totalPages;
            }

            void setTotalPages(int totalPages)
            {
                totalPages = totalPages;
            }
            
            E_DOCUMENT_TYPE getType() const
            {
                size_t dot = filename.rfind('.');
                std::experimental::string_view tmp{&filename[dot], dot-filename.size()};
                
                if(tmp == "pdf")  return EDT_PDF;
                if(tmp == "ps")   return EDT_POSTSCRIPT;
                if(tmp == "png")  return EDT_IMAGE;
                if(tmp == "jpg")  return EDT_IMAGE;
                if(tmp == "jpeg") return EDT_IMAGE;
                if(tmp == "bmp")  return EDT_IMAGE;
                if(tmp == "txt")  return EDT_TEXT;
                
                return EDT_UNKNOW;
            }
        protected:
            std::string filename;
            int totalPages;
    };
}
