#ifndef DEV_STORAGE_VIEW_HPP
#define DEV_STORAGE_VIEW_HPP

#include "view/text_view.hpp"
#include "game/storage.hpp"

class TextView;
class WorldData;
class Camera;

namespace dev {

class StorageView : public View, public View::OnClickListener, public Storage::OnStorageChangeListener
{
  public:
	StorageView(View *parent, Camera *cam, WorldData *world);
	bool onClick(float x, float y) override;
	bool onClick(View *view, float x, float y) override;
	void onStorageChange(Storage *storage) override;

  private:
	Storage *watched;
	WorldData *world;
	TextView *txt;
	Camera *cam;
	View *root;
};

} // namespace dev

#endif /* DEV_STORAGE_VIEW_HPP */