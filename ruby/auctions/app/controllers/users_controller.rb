class UsersController < ApplicationController
  before_filter :signed_in, only: [:edit, :update]
  before_filter :correct_user, only: [:edit, :update]

  def new
    @user = User.new
  end

  def create
    @user = User.new(params[:user])
    if @user.save
      flash[:succed] = "Account created successfully."
      redirect_to @user
    else
      flash[:error] = "#{@user.errors.count} errors, cannot proceed."
      render 'new'
    end
  end

  def show
    @user = User.find(params[:id])
  end

  def edit
  end

  def update
    if @user.update_attributes(params[:user])
      flash[:success] = 'Profile updated successfully.'
      sign_in @user
      redirect_to @user
    else
      flash[:error] = 'Incorrect informations.'
      render 'edit'
    end
  end

  def index
    @users = User.all
  end

  private

  def signed_in
    unless signed_in?
      flash[:notice] = 'Only signed in users have acccess to this site'
      redirect_to signin_path
    end
  end

  def correct_user
    @user = User.find(params[:id])
    unless current_user?(@user)
      flash[:notice] = 'You are not allowed to view this page.'
      redirect_to root_path
    end
  end
  
end
