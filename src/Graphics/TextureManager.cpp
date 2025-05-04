
#include "TextureManager.h"
#include "Game.h"
#include "Camera.h"
#include "tinyxml.h"
TextureManager* TextureManager::s_Instance = nullptr;
bool TextureManager::Load(std::string id, std::string filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (surface == nullptr) {
        SDL_Log("Failed to load image %s: %s", filename.c_str(), SDL_GetError());
        return false;
    }

    SDL_Log("✅ Loaded image: %s", filename.c_str());

    // Kiểm tra nếu ảnh có transparency (alpha)
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0)); // Đặt màu đen thành trong suốt

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::GetInstance()->GetRenderer(), surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr) {
        SDL_Log("❌ Failed to create texture from %s", filename.c_str());
        return false;
    }

    m_TextureMap[id] = texture;
    return true;
}
void TextureManager::Draw(std::string id, int x, int y, int width, int height, float scaleX, float scaleY, float scrollRatio, SDL_RendererFlip flip) {
    if (m_TextureMap.find(id) == m_TextureMap.end()) {
        //SDL_Log("❌ Texture ID %s not found!", id.c_str());
        return;
    }

    if (m_TextureMap[id] == nullptr) {
       // SDL_Log("❌ Texture ID %s is NULL!", id.c_str());
        return;
    }

    SDL_Rect srcRect = {0, 0, width, height};
    SDL_Rect dstRect;

    if (id == "bg") {
        // Background cố định, không bị ảnh hưởng bởi camera
        dstRect = {x, y, width, height};
    } else {
        // Các đối tượng khác di chuyển theo camera
        Vector2D cam = Camera::GetInstance()->GetPosition()*scrollRatio;
        dstRect = {x - cam.X, y - cam.Y, width*scaleX, height*scaleY};
    }

    if (SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip) != 0) {
        SDL_Log("❌ SDL_RenderCopyEx error: %s", SDL_GetError());
    } else {
        SDL_Log("✅ Drawing texture: %s at (%d, %d)", id.c_str(), x, y);
    }
}
void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip) {
    if (m_TextureMap.find(id) == m_TextureMap.end()) {
        //SDL_Log("Texture ID %s not found!", id.c_str());
        return;
    }

    SDL_Rect srcRect = {width * frame, height * (row - 1), width, height};
     Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = {x-cam.X, y-cam.Y, width, height};

    SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawTile(std::string tilesetID, int tileSize, int x, int y, int row, int col, SDL_RendererFlip flip) {
    // Tính toán srcRect: Phần của tileset cần vẽ
    SDL_Rect srcRect = {
        col * tileSize,  // Vị trí cột (x)
        row * tileSize,  // Vị trí hàng (y)
        tileSize,        // Chiều rộng (tileSize)
        tileSize         // Chiều cao (tileSize)
    };
     //Vector2D cam = Camera::GetInstance()->GetPosition();
     Vector2D cam = Camera::GetInstance()->GetPosition();
    // Tính toán dstRect: Vị trí và kích thước trên màn hình
    SDL_Rect dstRect = {
        x-cam.X,               // Vị trí x trên màn hình
        y-cam.Y,               // Vị trí y trên màn hình
        tileSize,        // Chiều rộng (tileSize)
        tileSize         // Chiều cao (tileSize)
    };


    // Vẽ tile lên màn hình
    SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[tilesetID], &srcRect, &dstRect, 0, nullptr, flip);
}
void TextureManager::Drop(std::string id) {
    if (m_TextureMap.find(id) != m_TextureMap.end()) {
        SDL_DestroyTexture(m_TextureMap[id]);
        m_TextureMap.erase(id);
    }
}

void TextureManager::Clean() {
    for (auto& texture : m_TextureMap) {
        SDL_DestroyTexture(texture.second);
    }
    m_TextureMap.clear();
    SDL_Log("Texture map cleaned!");
}/*
bool TextureManager::ParseTextures(std::string source)
{
    TiXmlDocument xml;
    xml.LoadFile(source);
    if(xml.Error()){
        std::cout << "Failed to Load: " << source << std::endl;
        return false;
    }

    TiXmlElement* root = xml.RootElement();
    for(TiXmlElement* e = root->FirstChildElement(); e!=nullptr;e=e->NextSiblingElement()){
        if(e->Value() == std::string("texture")){
            std::string id = e -> Attribute("id");
            std::string src = e->Attribute("source");
            Load(id, src);
        }
    }
    return true;
}
*/
bool TextureManager::ParseTextures(std::string source)
{
    TiXmlDocument xml;
    if (!xml.LoadFile(source)) {
        std::cout << "Failed to load: " << source << std::endl;
        return false;
    }

    TiXmlElement* root = xml.RootElement();
    if (root == nullptr) {
        std::cout << "Root element is missing in the XML file." << std::endl;
        return false;
    }

    // Duyệt qua các phần tử con của root
    for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
        // Kiểm tra nếu tên thẻ là "Texture"
        if (e->ValueStr() == "Texture") {
            const char* id = e->Attribute("id");
            const char* src = e->Attribute("source");

            if (id && src) {
                Load(id, src); // Giả sử hàm Load nhận đúng 2 tham số này.
            } else {
                std::cout << "Missing attributes in a <Texture> element." << std::endl;
            }
        }
    }
    return true;
}
