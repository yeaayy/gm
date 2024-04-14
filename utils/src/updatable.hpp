#ifndef UTILS_UPDATABLE_HPP
#define UTILS_UPDATABLE_HPP

class Updatable {
  public:
    static void updateAll(float dt);
    void activateUpdate(bool = true);
    bool isUpdateActivated();

  protected:
    Updatable(bool active= false);
    virtual void update(float dt);

  private:
    bool isUpdateActivate;
};

#endif /* UTILS_UPDATABLE_HPP */
